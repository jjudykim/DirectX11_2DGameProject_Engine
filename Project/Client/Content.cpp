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
	m_Tree->ShowRoot(false);                                                 // * Root Node�� ������ �ʵ��� *
	m_Tree->UseDrag(true);       	                                         // Drag / Drop On
	m_Tree->ShowNameOnly(true);                                              // Show Name Only (��� / Ȯ���� X)
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Content::AssetClicked);    // Clicked Delegate ���
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&Content::PopupMenu);         // Popup Delegate ���

	// Asset�� ���¸� Content�� TreeUI�� �ݿ�
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
	// AssetMgr�� �������� �����, Ʈ�� ������ �籸��
	if (CAssetMgr::GetInst()->IsChanged())
	{
		RenewContent();
	}
}

void Content::RenewContent()
{
	// Tree �ʱ�ȭ
	m_Tree->Clear();

	// Parent Node�� ���� ��� -> Root Node�� �Է�
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

	// Asset Manager���� �ε��Ǿ�����, Content �������� ���� Asset���� ����
	// �ε��� Asset�� �ش��ϴ� ���� ������ Content ������ �ִ��� Exist üũ
	wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		const map<wstring, Ptr<CAsset>>& mapAsset = CAssetMgr::GetInst()->GetAssets((ASSET_TYPE)i);
		for (const auto pair : mapAsset)
		{
			// Engine Asset���� ���� ������ ���� ������ pass
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
					int ret = MessageBox(nullptr, L"�ٸ� ������ �����ǰ� ���� �� �ֽ��ϴ�. \n Asset�� �����Ͻðڽ��ϱ�?", L"���� ���� ����", MB_YESNO);
					if (ret == IDYES)
					{
						// Asset Delete ��û
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

	// Ž�� Ŀ�� ������Ʈ�� ���� ������ �ݺ��ؼ� ã�Ƴ�����
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