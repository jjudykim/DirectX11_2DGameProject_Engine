#include "pch.h"
#include "MenuUI.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Scripts/CScriptMgr.h>
#include <Engine/CGameObject.h>
#include <Engine/CScript.h>

#include "CEditorMgr.h"
#include "Inspector.h"
#include "CLevelSaveLoad.h"

MenuUI::MenuUI()
	: m_Popup(false)
{
}

MenuUI::~MenuUI()
{
}

void MenuUI::Tick()
{
	if (!IsActive())
		return;
	
	if (ImGui::BeginMainMenuBar())
	{
		Update();
		ImGui::EndMainMenuBar();
	}

	if (m_Popup)
		ImGui::OpenPopup("New Object");

	// Create Object Modal
	if (ImGui::BeginPopupModal("New Object", NULL, ImGuiWindowFlags_AlwaysAutoResize))
	{
		static char name[256] = "";
		static int selectItem = 0;
		ImGui::SeparatorText("Create New GameObject");

		ImGui::Text("Object Name");
		ImGui::SameLine(100.f);
		ImGui::InputText("##ObjectName", name, 256);

		ImGui::Text("Layer");
		ImGui::SameLine(100.f);

		ImGui::Combo("##LayerCombo", &selectItem, LAYER_TYPE_STRING, IM_ARRAYSIZE(LAYER_TYPE_STRING));

		ImGui::SetCursorPosX(ImGui::GetWindowWidth() - 200.f);
		if (ImGui::Button("Create", ImVec2(80.f, 18.f)))
		{
			if (strlen(name) == 0)
			{
				int i = 0;
			}
			else
			{
				CGameObject* pObject = new CGameObject;
				wchar_t wstrName[256];
				MultiByteToWideChar(CP_UTF8, 0, name, -1, wstrName, 256);
				pObject->SetName(wstrName);
				CLevelMgr::GetInst()->GetCurrentLevel()->AddObject(selectItem, pObject);

				memset(name, 0, 256);
			}
			m_Popup = false;
			ImGui::CloseCurrentPopup();
		}
		ImGui::SameLine(0.f, 5.f);
		if (ImGui::Button("Cancel", ImVec2(80.f, 18.f)))
		{

		}
		ImGui::EndPopup();
	}
}

void MenuUI::Update()
{
	File();
	Level();
	GameObject();
	Assets();
}

void MenuUI::File()
{
	if (ImGui::BeginMenu("File"))
	{
		if (ImGui::MenuItem("Level Save"))
		{
			WCHAR szSelect[256] = {};

			OPENFILENAME filename = {};

			filename.lStructSize = sizeof(filename);
			filename.hwndOwner = nullptr;
			filename.lpstrFile = szSelect;
			filename.lpstrFile[0] = '\0';
			filename.nMaxFile = sizeof(szSelect);
			filename.lpstrFilter = L"Level\0*.lv";
			filename.nFilterIndex = 1;
			filename.lpstrFileTitle = NULL;
			filename.nMaxFileTitle = 0;

			// 탐색창 초기 위치 지정
			wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
			strInitPath += L"level\\";
			filename.lpstrInitialDir = strInitPath.c_str();

			filename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetSaveFileName(&filename))
			{
				wstring path = filename.lpstrFile;
				CLevelSaveLoad::SaveLevel(path, CLevelMgr::GetInst()->GetCurrentLevel());
			}

		}

		if (ImGui::MenuItem("Level Load"))
		{
			wchar_t szSelect[256] = {};

			OPENFILENAME filename = {};

			filename.lStructSize = sizeof(filename);
			filename.hwndOwner = nullptr;
			filename.lpstrFile = szSelect;
			filename.lpstrFile[0] = '\0';
			filename.nMaxFile = sizeof(szSelect);
			filename.lpstrFilter = L"Level\0*.lv";
			filename.nFilterIndex = 1;
			filename.lpstrFileTitle = NULL;
			filename.nMaxFileTitle = 0;

			// 탐색창 초기 위치 지정
			wstring strInitPath = CPathMgr::GetInst()->GetContentPath();
			strInitPath += L"level\\";
			filename.lpstrInitialDir = strInitPath.c_str();

			filename.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

			if (GetOpenFileName(&filename))
			{
				wstring path = filename.lpstrFile;
				CLevel* pLevel = CLevelSaveLoad::LoadLevel(path);
				ChangeLevel(pLevel, LEVEL_STATE::STOP);

				// Inspector 의 타겟정보를 nullptr 로 되돌리기
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
				pInspector->SetTargetObject(nullptr);
			}
		}

		ImGui::EndMenu();
	}
}

void MenuUI::Level()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	
	ImGui::BeginDisabled(!pCurLevel);

	if (ImGui::BeginMenu("Level"))
	{
		LEVEL_STATE State = LEVEL_STATE::STOP;
		if (pCurLevel != nullptr)
			State = pCurLevel->GetState();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY == State);
		if (ImGui::MenuItem("Play"))
		{
			if (LEVEL_STATE::STOP == State)
			{
				wstring strLevelPath = CPathMgr::GetInst()->GetContentPath();
				strLevelPath += L"level\\Temp.lv";
				CLevelSaveLoad::SaveLevel(strLevelPath, pCurLevel);
			}

			ChangeLevelState(LEVEL_STATE::PLAY);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::PLAY != State);
		if (ImGui::MenuItem("Pause"))
		{
			ChangeLevelState(LEVEL_STATE::PAUSE);
		}
		ImGui::EndDisabled();

		ImGui::BeginDisabled(LEVEL_STATE::STOP == State);
		if (ImGui::MenuItem("Stop"))
		{
			wstring StrLevelLoadPath = CPathMgr::GetInst()->GetContentPath();
			StrLevelLoadPath += L"level\\Temp.lv";
			CLevel* pLoadedLevel = CLevelSaveLoad::LoadLevel(StrLevelLoadPath);
			ChangeLevel(pLoadedLevel, LEVEL_STATE::STOP);

			// Inspector Clear
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			pInspector->SetTargetObject(nullptr);
			pInspector->SetTargetAsset(nullptr);
		}
		ImGui::EndDisabled();

		ImGui::EndMenu();
	}
	ImGui::EndDisabled();
}

void MenuUI::GameObject()
{
	if (ImGui::BeginMenu("GameObject"))
	{
		if (ImGui::MenuItem("Create Empty Object"))
		{
			m_Popup = true;
		}
			
		if (ImGui::BeginMenu("Add Component"))
		{
			// TargetObject
			Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
			CGameObject* pObject = pInspector->GetTargetObject();

			if (ImGui::MenuItem("1. Transform"))
				pObject->AddComponent(new CTransform);

			if (ImGui::MenuItem("2. Collider2D"))
			{
				pObject->AddComponent(new CCollider2D);
				if (pObject->GetComponent(COMPONENT_TYPE::TRANSFORM) == nullptr)
					pObject->AddComponent(new CTransform);
			}
				
			if (ImGui::MenuItem("3. FlipBook"))
				pObject->AddComponent(new CFlipBookComponent);

			if (ImGui::MenuItem("4. FSM"))
				pObject->AddComponent(new CFSM);

			if (ImGui::MenuItem("5. RigidBody"))
			{
				pObject->AddComponent(new CRigidBody);
				if (pObject->GetComponent(COMPONENT_TYPE::TRANSFORM) == nullptr)
					pObject->AddComponent(new CTransform);
			}
				
			if (ImGui::MenuItem("6. Camera"))
			{
				pObject->AddComponent(new CCamera);
				if (pObject->GetComponent(COMPONENT_TYPE::TRANSFORM) == nullptr)
					pObject->AddComponent(new CTransform);
			}
				
			if (ImGui::MenuItem("7. Light2D"))
			{
				pObject->AddComponent(new CLight2D);
				if (pObject->GetComponent(COMPONENT_TYPE::TRANSFORM) == nullptr)
					pObject->AddComponent(new CTransform);
					
			}
				
			ImGui::SeparatorText("Render Component");

			if (ImGui::MenuItem("MeshRender"))
				pObject->AddComponent(new CMeshRender);

			if (ImGui::MenuItem("TileMap"))
				pObject->AddComponent(new CTileMap);

			ImGui::EndMenu();
		}

		AddScript();

		ImGui::EndMenu();
	}
}

void MenuUI::Assets()
{
	if (ImGui::BeginMenu("Assets"))
	{
		if (ImGui::MenuItem("Create Empty Material"))
		{
			Ptr<CMaterial> pMtrl = new CMaterial;
			wstring Key = GetAssetKey(ASSET_TYPE::MATERIAL, L"Default Material");
			CAssetMgr::GetInst()->AddAsset<CMaterial>(Key, pMtrl);
			pMtrl->Save(Key);
		}

		EditorUI* pSpriteEditor = CEditorMgr::GetInst()->FindEditorUI("SpriteEditor");
		EditorUI* pFlipBookEditor = CEditorMgr::GetInst()->FindEditorUI("FlipBookEditor");

		bool SE_IsActive = pSpriteEditor->IsActive();
		bool FE_IsActive = pFlipBookEditor->IsActive();

		if (ImGui::MenuItem("Sprite Editor", nullptr, &SE_IsActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("SpriteEditor")->SetActive(SE_IsActive);
		}

		if (ImGui::MenuItem("FlipBook Editor", nullptr, &FE_IsActive))
		{
			CEditorMgr::GetInst()->FindEditorUI("FlipBookEditor")->SetActive(FE_IsActive);
		}

		ImGui::EndMenu();
	}
}

void MenuUI::AddScript()
{
	if (ImGui::BeginMenu("Add Script"))
	{
		vector<wstring> vecScriptsName;
		CScriptMgr::GetScriptInfo(vecScriptsName);

		for (size_t i = 0; i < vecScriptsName.size(); ++i)
		{
			if (ImGui::MenuItem(string(vecScriptsName[i].begin(), vecScriptsName[i].end()).c_str()))
			{
				// Insepctor
				Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");

				// TargetObject
				CGameObject* pObject = pInspector->GetTargetObject();
				if (pObject != nullptr)
				{
					// TargetObject에 선택한 Script를 추가
					CScript* pScript = CScriptMgr::GetScript(vecScriptsName[i]);
					pObject->AddComponent(pScript);
				}
			}
		}

		ImGui::EndMenu();
	}
}

wstring MenuUI::GetAssetKey(ASSET_TYPE _Type, const wstring& _KeyFormat)
{
	wstring Key;

	switch (_Type)
	{
	case ASSET_TYPE::MATERIAL:
		Key = wstring(L"material\\") + _KeyFormat + L" %d.mtrl";
		break;
	case ASSET_TYPE::PREFAB:
		Key = wstring(L"prefab\\") + _KeyFormat + L" %d.pref";
		break;
	case ASSET_TYPE::SPRITE:
		Key = wstring(L"sprite\\") + _KeyFormat + L" %d.sprite";
		break;
	case ASSET_TYPE::FLIPBOOK:
		Key = wstring(L"flipbook\\") + _KeyFormat + L" %d.flip";
		break;
	default:
		assert(nullptr);
		break;
	}

	wchar_t szKey[255] = {};
	wstring FilePath = CPathMgr::GetInst()->GetContentPath();

	for (UINT i = 0; i < 0xffffffff; ++i)
	{
		swprintf_s(szKey, 255, Key.c_str(), i);

		if (std::filesystem::exists(FilePath + szKey) == false)
		{
			break;
		}
	}

	return szKey;
}


