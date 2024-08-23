#include "pch.h"
#include "LevelViewer.h"

#include "CLevelSaveLoad.h"

#include "CEditorMgr.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CPathMgr.h>
#include <Engine/CLevel.h>

static char cLevelName[256] = "";
static wchar_t wLevelName[256] = L"";

LevelViewer::LevelViewer()
	: m_LevelEditMode(false)
{
}

LevelViewer::~LevelViewer()
{
}


void LevelViewer::Update()
{	
	if (CLevelMgr::GetInst()->IsLevelChanged())
		RenewLevel();

	Viewer();
}

void LevelViewer::Viewer()
{ 
	WideCharToMultiByte(CP_UTF8, 0, m_LevelName.c_str(), -1, cLevelName, 256, NULL, NULL);

	UINT Flag = 0;

	if (m_LevelEditMode)
	{
		//Flag |= ImGuiInputTextFlags_ReadOnly;
	}
		
	ImGui::Text("Current Level");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(180.f);

	if (Flag != 0)
		ImGui::BeginDisabled();
	ImGui::InputText("##LevelFile", cLevelName, IM_ARRAYSIZE(cLevelName), Flag);
	if (Flag != 0)
		ImGui::EndDisabled();

	MultiByteToWideChar(CP_UTF8, 0, cLevelName, -1, wLevelName, 256);
	m_LevelName = wLevelName;

	ViewerButton();
}

void LevelViewer::ViewerButton()
{
	if (m_LevelEditMode)
	{
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("Save", ImVec2(50.f, 18.f)))
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
				m_LevelEditMode = false;
			}
		}
	}
	else
	{
		ImGui::SameLine(0.f, 10.f);
		if (ImGui::Button("New", ImVec2(50.f, 18.f)))
		{
			if (m_LevelName != L"")
			{
				m_CurLevel = new CLevel;
				m_CurLevel->SetName(m_LevelName);

				WCHAR szSelect[256] = {};
				wcscpy_s(szSelect, (m_LevelName + L".lv").c_str());

				OPENFILENAME filename = {};

				filename.lStructSize = sizeof(filename);
				filename.hwndOwner = nullptr;
				filename.lpstrFile = szSelect;
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
					CLevelSaveLoad::SaveLevel(path, m_CurLevel);
					ChangeLevel(m_CurLevel, LEVEL_STATE::STOP);
					m_LevelEditMode = true;
				}
			}
				
		}
		ImGui::SameLine(0.f, 5.f);
		if (ImGui::Button("Load", ImVec2(50.f, 18.f)))
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

				m_LevelEditMode = true;
			}
		}
	}
}

void LevelViewer::RenewLevel()
{
	m_CurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (m_CurLevel != nullptr)
	{
		m_LevelName = m_CurLevel->GetName();
		m_LevelEditMode = true;
	}
		
}