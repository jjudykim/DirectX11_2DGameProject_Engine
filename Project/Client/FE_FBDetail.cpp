#include "pch.h"
#include "FE_FBDetail.h"

#include "FE_FBViewer.h"
#include "FE_SpriteList.h"
#include <Engine/CAssetMgr.h>
#include "CEditorMgr.h"
#include "ListUI.h"

FE_FBDetail::FE_FBDetail()
	: m_Mode{0, 0}
	, m_FPS(0)
	, m_Repeat(0)
	, m_CurSpriteIndex(-1)
	, m_CurFlipBookName("")
	, m_MaxSpriteIndex(0)
	, m_IsActive(0)
{
}

FE_FBDetail::~FE_FBDetail()
{
}

void FE_FBDetail::Init()
{
	m_CurFlipBook = nullptr;
	m_CurSprite = nullptr;
	m_AtlasTex = nullptr;

	m_CurFlipBookName = "";
	m_CurSpriteName = "";
	m_FPS = 0;
	m_Offset = Vec2(0, 0);
	m_Repeat = false;
	m_CurSpriteIndex = -1;
	m_MaxSpriteIndex = 0;

	m_Mode[0] = 0;
	m_Mode[1] = 0;
	m_IsActive = false;
}

void FE_FBDetail::SetFlipBookData()
{
	GetSpriteList()->SetSpriteToList(m_CurFlipBook->GetVecSprite());
	m_CurFlipBookName = string(m_CurFlipBook->GetKey().begin(), m_CurFlipBook->GetKey().end());
	m_CurSprite = m_CurFlipBook->GetSprite(0);
	m_AtlasTex = m_CurSprite->GetAtlasTexture();
	m_FPS = m_CurFlipBook->GetFPS();
	m_Offset = m_CurSprite->GetOffsetUV() * Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());
	m_CurSpriteIndex = 0;
	m_MaxSpriteIndex = m_CurFlipBook->GetMaxFrameCount();

}

void FE_FBDetail::SetSpriteData()
{
	m_CurSprite = GetSpriteList()->GetCurSprite();
	path Path = m_CurSprite->GetKey();
	m_CurSpriteName = Path.stem().string();
	m_AtlasTex = m_CurSprite->GetAtlasTexture();
	m_CurSpriteIndex = GetSpriteList()->GetCurSpriteIndex();
	m_MaxSpriteIndex = GetSpriteList()->GetSpriteCount();
	m_Offset = m_CurSprite->GetOffsetUV() * Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());
}

void FE_FBDetail::Update()
{
	if (m_Mode[0] || m_Mode[1])
		m_IsActive = true;
	else
		m_IsActive = false;

	SelectMode();
	FlipBookInfo();
	SpriteInfo();

	if (m_Mode[0] || m_Mode[1])
		SaveFlipBook();

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
			GetViewer()->Init();
			GetSpriteList()->Init();
			Init();
		}
	}
	else if (m_Mode[1])
	{
		ImGui::TextColored(ImVec4(0.5f, 0.7f, 1.0f, 1.0f), "[Edit Mode]");
		ImGui::TextDisabled(": Edit Existing FlipBook Mode");
		ImGui::SameLine(ImGui::GetWindowSize().x - 90.f);
		if (ImGui::Button("Cancel", ImVec2(80.f, 18.f)))
		{
			GetViewer()->Init();
			GetSpriteList()->Init();
			Init();
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
	ImGui::SeparatorText("");

	if (!m_IsActive)
		ImGui::BeginDisabled();

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 105.f);
	if (ImGui::Button("Play", ImVec2(100.f, 22.f)))
	{
		m_IsPlaying = true;
	}
	ImGui::SameLine(0.f, 10.f);
	if (ImGui::Button("Stop", ImVec2(100.f, 22.f)))
	{
		m_IsPlaying = false;
		m_CurSpriteIndex = 0;
		GetSpriteList()->SetCurSpriteIndex(m_CurSpriteIndex);
		GetViewer()->SetCurSprite(m_CurSprite);
	}

	if (!m_IsActive)
		ImGui::EndDisabled();

	if (m_IsPlaying)
	{
		float MaxTime = 1.f / m_FPS;
		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurSpriteIndex;

			if (m_MaxSpriteIndex <= m_CurSpriteIndex)
			{
				if (m_Repeat)
					m_CurSpriteIndex = 0;
				else
				{
					m_IsPlaying = false;
					m_CurSpriteIndex = 0;
				}
			}

			GetSpriteList()->SetCurSpriteIndex(m_CurSpriteIndex);
			GetViewer()->SetCurSprite(m_CurSprite);
		}

		m_AccTime += EngineDT;
	}
}

void FE_FBDetail::SelectFlipBook(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		GetViewer()->Init();
		GetSpriteList()->Init();
		Init();
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(strAssetName);

	assert(pFlipBook.Get());

	m_CurFlipBook = pFlipBook;
	SetFlipBookData();
}

void FE_FBDetail::FlipBookInfo()
{
	ImGui::SeparatorText("FlipBook Information");

	if (!m_IsActive)
		ImGui::BeginDisabled();

	ImGui::Text("FlipBook Name");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	if (ImGui::InputText("##FlipBookName", m_CurFlipBookName.data(), 255))
	{
		m_CurFlipBookName.resize(strlen(m_CurFlipBookName.c_str()));
	};

	ImGui::Text("");

	// Frame Index
	int frameCount = m_CurSpriteIndex + 1;
	ImGui::Text("Frame Count");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(30);
	ImGui::InputInt("##CurFrameCount", &frameCount, 0, 0, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine();
	ImGui::Text("/");
	ImGui::SameLine();
	ImGui::Text(std::to_string(m_MaxSpriteIndex).c_str());
	ImGui::SameLine(257.f);

	int leftIndex = m_CurSpriteIndex;
	if (m_MaxSpriteIndex < 1 || leftIndex == 0)
		ImGui::BeginDisabled();
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{ 
		if (m_CurSpriteIndex > 0)
			m_CurSpriteIndex--;

		GetSpriteList()->SetCurSpriteIndex(m_CurSpriteIndex);
		GetViewer()->SetCurSprite(m_CurSprite);
	}
	if (m_MaxSpriteIndex < 1 || leftIndex == 0)
		ImGui::EndDisabled();

	ImGui::SameLine(0.0f, 5.f);

	int index = m_CurSpriteIndex;
	if (index == m_MaxSpriteIndex - 1)
		ImGui::BeginDisabled();
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{ 
		m_CurSpriteIndex++;
		if (m_MaxSpriteIndex <= m_CurSpriteIndex)
			m_CurSpriteIndex = m_MaxSpriteIndex - 1;

		GetSpriteList()->SetCurSpriteIndex(m_CurSpriteIndex);
		GetViewer()->SetCurSprite(m_CurSprite);
	}
	if (index == m_MaxSpriteIndex - 1)
		ImGui::EndDisabled();

	// FPS
	ImGui::Text("FPS");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputInt("##FlipBookFPS", &m_FPS);
	if (m_FPS < 0) m_FPS = 0;

	// Repeat
	ImGui::Text("Repeat");
	ImGui::SameLine(120);
	ImGui::Checkbox("##FlipBookRepeat", &m_Repeat);

	ImGui::Text("");

	if (!m_IsActive)
		ImGui::EndDisabled();
}

void FE_FBDetail::SpriteInfo()
{
	ImGui::SeparatorText("Sprite Information");

	if (!m_IsActive)
		ImGui::BeginDisabled();

	ImGui::Text("Current Sprite");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputText("##CurSpriteName", (char*)m_CurSpriteName.c_str(), 255);

	ImGui::Text("");

	float size[2] = { 0, 0 };
	if (m_CurSprite != nullptr)
	{
		size[0] = m_CurSprite->GetBackgroundUV().x * m_AtlasTex->Width();
		size[1] = m_CurSprite->GetBackgroundUV().y * m_AtlasTex->Height();
	}
	ImGui::Text("Sprite Size");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputFloat2("##CurSpriteSize", size, "%.2f");

	ImGui::Text("Offset");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(85);
	if (ImGui::InputFloat("##CurSpriteOffsetX", &m_Offset.x, 1.0f, 0.0f, "%.2f"))
		m_CurSprite->SetOffset(m_Offset);
	ImGui::SameLine();
	ImGui::SetNextItemWidth(85);
	if (ImGui::InputFloat("##CurSpriteOffsetY", &m_Offset.y, 1.0f, 0.0f, "%.2f"))
		m_CurSprite->SetOffset(m_Offset);
	

	ImGui::Text("");

	if (!m_IsActive)
		ImGui::EndDisabled();
}

void FE_FBDetail::SaveFlipBook()
{
	static int clicked = 0;
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 50.f);
	if (ImGui::Button("Save", ImVec2(100.f, 18.f)))
		clicked++;

	if (0 < clicked)
	{
		if (m_CurFlipBookName.empty() || m_FPS == 0 || m_MaxSpriteIndex < 1)
		{
			ImGui::TextColored(ImVec4(1.0f, 0.5f, 0.5f, 1.0f), "**Please Check the elements for FlipBooks**");
		}
		else
		{
			ImGui::OpenPopup("Save FlipBook");

			ImVec2 center = ImGui::GetMainViewport()->GetCenter();
			ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

			if (ImGui::BeginPopupModal("Save FlipBook", NULL, ImGuiWindowFlags_AlwaysAutoResize))
			{
				ImGui::Text("Are you sure you want to save it?");
				ImGui::Text("");
				ImGui::SeparatorText("FlipBook's Setted Elements");
				ImGui::Text("FlipBookName : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.5, 1.0f, 0.8f, 1.0f), m_CurFlipBookName.c_str());
				ImGui::Text("Sprite Count : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.5, 1.0f, 0.8f, 1.0f), std::to_string(m_MaxSpriteIndex).c_str());
				ImGui::Text("FPS : ");
				ImGui::SameLine();
				ImGui::TextColored(ImVec4(0.5, 1.0f, 0.8f, 1.0f), std::to_string(m_FPS).c_str());

				if (ImGui::Button("Save", ImVec2(120, 0))) 
				{ 
					clicked = 0;

					m_CurFlipBook->SetFPS(m_FPS);
					for (int i = 0; i < GetSpriteList()->GetSpriteCount(); ++i)
					{
						m_CurFlipBook->AddSprite(GetSpriteList()->m_vecAddedSprite[i]);
					}
					
					wstring Path = CPathMgr::GetInst()->GetContentPath();
					Path += wstring(L"animation\\") + wstring(m_CurFlipBookName.begin(), m_CurFlipBookName.end());
					Path += L".flip";
					m_CurFlipBook->Save(Path);
					
					GetViewer()->Init();
					GetSpriteList()->Init();
					Init();

					ImGui::CloseCurrentPopup(); 
				}
				ImGui::SetItemDefaultFocus();
				ImGui::SameLine();
				if (ImGui::Button("Cancel", ImVec2(120, 0))) 
				{
					clicked = 0;
					ImGui::CloseCurrentPopup(); 
				}
				ImGui::EndPopup();
			}
		}
	}

	
	
	
}
