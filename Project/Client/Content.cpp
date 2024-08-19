#include "pch.h"
#include "Content.h"

#include "CEditorMgr.h"
#include "TreeUI.h"
#include "Inspector.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>
#include <Engine/CTaskMgr.h>

Content::Content()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("ContentTree");
	AddChild(m_Tree);

	// Tree Option Setting
	m_Tree->ShowRoot(false);                                                 // * Root Node가 보이지 않도록 *
	m_Tree->UseDrag(true);       	                                         // Drag / Drop On
	m_Tree->ShowNameOnly(true);                                              // Show Name Only (경로 / 확장자 X)
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);    // Clicked Delegate 등록
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&Content::PopupMenu);         // Popup Delegate 등록

	// Asset의 상태를 Content의 TreeUI에 반영
	RenewContent();
}

Content::~Content()
{
}

void Content::Init()
{
	Reload();
}

void Content::Update()
{
	// AssetMgr에 변경점이 생기면, 트리 내용을 재구성
	if (CAssetMgr::GetInst()->IsChanged())
	{
		RenewContent();
	}
}

void Content::RenewContent()
{
	// Tree 초기화
	m_Tree->Clear();

	// Parent Node가 없을 경우 -> Root Node로 입력
	TreeNode* pRoot = m_Tree->AddNode(nullptr, "Root");

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		TreeNode* pNode = m_Tree->AddNode(pRoot, ToString((ASSET_TYPE)i));
		pNode->SetFrame(true);

		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		
		if (i == (UINT)ASSET_TYPE::SPRITE)
		{
			map<string, TreeNode*> mapDirNode = {};

			for (const auto& pair : mapAsset)
			{
				m_Tree->AddNodeByDir(pNode, string(pair.first.begin(), pair.first.end()), mapDirNode, pair.second.Get());
			}
		}
		else
		{
			for (const auto& pair : mapAsset)
			{
				m_Tree->AddNode(pNode, string(pair.first.begin(), pair.first.end()), (DWORD_PTR)pair.second.Get());
			}
		}
	}
}

void Content::AssetClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	if (pNode->IsFrame())
		return;

	if (pNode->IsCategory())
		return;

	Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetAsset(pAsset);
	//ImGui::SetWindowFocus(nullptr);
}

void Content::Reload()
{
	wstring ContentPath = CPathMgr::GetInst()->GetContentPath();
	FindAssetName(ContentPath, L"*.*");

	// All Path Asset Loading -> Add Asset Manager
	for (size_t i = 0; i < m_vecAssetPath.size(); ++i)
	{
		LoadAsset(m_vecAssetPath[i]);
	}

	// Asset Manager에는 로딩되었지만, Content 폴더에는 없는 Asset들을 삭제
	// 로딩된 Asset에 해당하는 원본 파일이 Content 폴더에 있는지 Exist 체크
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			// Engine Asset들은 원본 파일이 없기 때문에 pass
			if (pair.second->IsEngineAsset())
				continue;
			
			wstring strRelativePath = pair.second->GetRelativePath();

			if (std::filesystem::exists(strContentPath + strRelativePath) == false)
			{
				if (pair.second->GetRefCount() <= 1)
				{
					// Asset Delete
					CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
				}
				else
				{
					int ret = MessageBox(nullptr, L"다른 곳에서 참조되고 있을 수 있습니다. \n Asset을 삭제하시겠습니까?", L"에셋 삭제 에러", MB_YESNO);
					if (ret == IDYES)
					{
						// Asset Delete 요청
						CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DEL_ASSET, (DWORD_PTR)pair.second.Get(), });
					}
				}
			}
		}
	}
}

void Content::FindAssetName(const wstring& _FolderPath, const wstring& _Filter)
{
	WIN32_FIND_DATA tFindData = {};

	// File / Folder search - kernel object
	wstring strFindPath = _FolderPath + _Filter;
	HANDLE hFinder = FindFirstFile(strFindPath.c_str(), &tFindData);
	assert(hFinder != INVALID_HANDLE_VALUE);

	// 탐색 커널 오브젝트로 다음 파일을 반복해서 찾아나가기
	while (FindNextFile(hFinder, &tFindData))
	{
		wstring strFindName = tFindData.cFileName;

		if (tFindData.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
		{
			if (strFindName == L"..")
				continue;

			FindAssetName(_FolderPath + strFindName + L"\\", _Filter);
		}
		else
		{
			wstring RelativePath = CPathMgr::GetInst()->GetRelativePath(_FolderPath + strFindName);
			m_vecAssetPath.push_back(RelativePath);
		}
	}

	FindClose(hFinder);
}

void Content::LoadAsset(const path& _Path)
{
	path ext = _Path.extension();

	if (ext == L".mesh")
		CAssetMgr::GetInst()->Load<CMesh>(_Path, _Path);
	//else if (ext == L".mdat")
		//CAssetMgr::GetInst()->Load<CMeshData>(_Path, _Path);
	else if (ext == L".mtrl")
		CAssetMgr::GetInst()->Load<CMaterial>(_Path, _Path);
	else if (ext == L".pref")
		CAssetMgr::GetInst()->Load<CPrefab>(_Path, _Path);
	else if (ext == L".png" || ext == L".jpg" || ext == L".jpeg" || ext == L".bmp" || ext == L".dds" || ext == L".tga"
		|| ext == L".PNG" || ext == L".JPG" || ext == L".JPEG" || ext == L".BMP" || ext == L".DDS" || ext == L".TGA")
		CAssetMgr::GetInst()->Load<CTexture>(_Path, _Path);
	//else if (ext == L".mp3" || ext == L".mp4" || ext == L".ogg" || ext == L".wav" 
	//	|| ext == L".MP3" || ext == L".MP4" || ext == L".OGG" || ext == L".WAV")
	//	CAssetMgr::GetInst()->Load<CSound>(_Path, _Path);
	else if (ext == L".sprite")
		CAssetMgr::GetInst()->Load<CSprite>(_Path, _Path);
	else if (ext == L".flip")
		CAssetMgr::GetInst()->Load<CFlipBook>(_Path, _Path);
}

void Content::PopupMenu(DWORD_PTR _Param)
{
	TreeNode* pTargetNode = (TreeNode*)_Param;

	Ptr<CAsset> pAsset = (CAsset*)pTargetNode->GetData();

	if (pAsset->GetAssetType() == ASSET_TYPE::PREFAB)
	{
		if (ImGui::MenuItem("Instantiate"))
		{
			Ptr<CPrefab> pPrefab = (CPrefab*)pAsset.Get();

			CGameObject* CloneObj = pPrefab->Instantiate();

			CreateObject(CloneObj, 0);

			ImGui::CloseCurrentPopup();
		}
	}

	// if (ImGui::Button("Close"))
	// ImGui::CloseCurrentPopup();
	ImGui::EndPopup();
}