#include "pch.h"
#include "FE_FBDetail.h"

#include <Engine/CAssetMgr.h>
#include "CEditorMgr.h"
#include "ListUI.h"

FE_FBDetail::FE_FBDetail()
	: m_Mode{0, 0}
	, m_FPS(0)
	, m_Repeat(0)
	, m_CurSpriteIndex(0)
	, m_MaxSpriteIndex(5)
{
}

FE_FBDetail::~FE_FBDetail()
{
}

void FE_FBDetail::Init()
{
}

void FE_FBDetail::SetFlipBookData()
{
}

void FE_FBDetail::Update()
{
	SelectMode();
	FlipBookInfo();
	SpriteInfo();
	FlipBookPlay();
}

void FE_FBDetail::SelectMode()
{
	if (m_Mode[0] || m_Mode[1])
	{
		ImGui::BeginDisabled();
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 105.f);
		ImGui::Button("New", ImVec2(100.f, 18.f));
		ImGui::SameLine(0.f, 10.f);
		ImGui::Button("Load", ImVec2(100.f, 18.f));
		ImGui::EndDisabled();
	}
	else
	{
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 105.f);
		if (ImGui::Button("New", ImVec2(100.f, 18.f)))
		{
			// Create New FlipBook
			m_Mode[0] = true;
			m_CurFlipBook = new CFlipBook;
		}

		ImGui::SameLine(0.f, 10.f);

		if (ImGui::Button("Load", ImVec2(100.f, 18.f)))
		{
			m_Mode[1] = true;
			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
			pListUI->SetShowNameOnly(true);
			pListUI->SetName("FlipBook");

			vector<string> vecFlipBookNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::FLIPBOOK, vecFlipBookNames);
			pListUI->AddList(vecFlipBookNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&FE_FBDetail::SelectFlipBook);
			pListUI->SetActive(true);
		}
	}

	if (m_Mode[0])
	{
		ImGui::TextColored(ImVec4(1.0f, 1.0f, 0.0f, 1.0f), "[Create Mode]");
		ImGui::TextDisabled(": Create New FlipBook Mode");
		ImGui::SameLine(ImGui::GetWindowSize().x - 90.f);
		if (ImGui::Button("Cancel", ImVec2(80.f, 18.f)))
		{
			m_CurFlipBook = nullptr;
			m_Mode[0] = false;
		}
	}
	else if (m_Mode[1])
	{
		ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.0f, 1.0f), "[Edit Mode]");
		ImGui::TextDisabled(": Edit Existing FlipBook Mode");
		ImGui::SameLine(ImGui::GetWindowSize().x - 90.f);
		if (ImGui::Button("Cancel", ImVec2(80.f, 18.f)))
		{
			m_CurFlipBook = nullptr;
			m_Mode[1] = false;
		}
	}
	else
	{
		ImGui::Text("");
		ImGui::Text("");
	}
}

void FE_FBDetail::FlipBookPlay()
{
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 105.f);
	if (ImGui::Button("Play", ImVec2(100.f, 18.f)))
	{
		// Play FlipBook
	}
	ImGui::SameLine(0.f, 10.f);
	if (ImGui::Button("Stop", ImVec2(100.f, 18.f)))
	{
		// Stop FlipBook
	}
}

void FE_FBDetail::SelectFlipBook(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_Mode[1] = false;
		m_CurFlipBook = nullptr;
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(strAssetName);

	assert(pFlipBook.Get());

	m_CurFlipBook = pFlipBook;
}

void FE_FBDetail::FlipBookInfo()
{
	ImGui::SeparatorText("FlipBook Information");

	string FlipBookName;

	if (m_Mode[1] && m_CurFlipBook != nullptr)
	{
		path Path = m_CurFlipBook->GetKey();
		FlipBookName = Path.stem().string();
	}
	ImGui::Text("FlipBook Name");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputText("##FlipBookName", (char*)FlipBookName.c_str(), 255);

	ImGui::Text("");

	// Frame Index
	ImGui::Text("Frame Index");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(30);
	ImGui::InputInt("##CurFrameIndex", &m_CurSpriteIndex, 0, 0, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	ImGui::Text("/");
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_MaxSpriteIndex).c_str());
	ImGui::SameLine(0.f, 77.f);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{ 
		m_CurSpriteIndex--;
		if (m_CurSpriteIndex < 0)
			m_CurSpriteIndex = 0;
	}
	ImGui::SameLine(0.0f, 5.f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{ 
		m_CurSpriteIndex++;
		if (m_MaxSpriteIndex < m_CurSpriteIndex)
			m_CurSpriteIndex = m_MaxSpriteIndex;
	}


	// FPS
	ImGui::Text("FPS");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputInt("##FlipBookFPS", &m_FPS);

	// Repeat
	ImGui::Text("Repeat");
	ImGui::SameLine(120);
	ImGui::Checkbox("##FlipBookRepeat", &m_Repeat);

	ImGui::Text("");
}

void FE_FBDetail::SpriteInfo()
{
	string SpriteName;

	ImGui::SeparatorText("Sprite Information");

	ImGui::Text("Current Sprite");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputText("##CurSpriteName", (char*)SpriteName.c_str(), 255);
}