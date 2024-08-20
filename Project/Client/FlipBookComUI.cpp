#include "pch.h"
#include "FlipBookComUI.h"

#include "CEditorMgr.h"
#include "TreeUI.h"
#include "ListUI.h"

#include <Engine/CSprite.h>
#include <Engine/CTexture.h>
#include <Engine/CFlipBook.h>

FlipBookComUI::FlipBookComUI()
	: ComponentUI(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
	, m_FBCom(nullptr)
	, m_UIHeight(0)
	, m_AccTime(0)
	, m_CurFrmIdx(0)
	, m_Playing(false)
	, m_Repeat(false)
	, m_AddMode(false)
{
}

FlipBookComUI::~FlipBookComUI()
{
}

void FlipBookComUI::Init()
{
	m_FBCom = GetTargetObject()->FlipBookComponent();
	m_CurFB = m_FBCom->GetCurFlipBook();
	m_SelectFB = nullptr;
	m_CurSprite = m_CurFB->GetSprite(0);

	m_UIHeight = 0;
	
	m_AccTime = 0;

	const vector<Ptr<CFlipBook>>& vecFlipBook = m_FBCom->GetVecFlipBook();
	for (size_t i = 0; i < vecFlipBook.size(); ++i)
	{
		if (vecFlipBook[i].Get() == nullptr)
			continue;

		if (vecFlipBook[i]->GetKey() == m_CurFB->GetKey())
		{
			m_ListIdx = i;
			break;
		}
		
		m_ListIdx = 0;
	}
	m_CurFrmIdx = 0;
	m_Repeat = false;
	m_Playing = false;
	m_AddMode = false;
}

void FlipBookComUI::Update()
{
	if (m_FBCom == nullptr)
		Init();

	m_UIHeight = 0;

	Title();
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	FlipBookList();
	EditFlipBookList();
	CurFlipBookInfo();

	if (m_AddMode)
		AddFlipBookPopup();

	m_UIHeight += 30.f;

	if (m_FBCom == nullptr || m_CurFB.Get() == nullptr)
	{
		SetChildSize(ImVec2(0.f, (float)m_UIHeight));
		return;
	}

	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Show Current FlipBook Sprite
	ShowFlipBookSprite(m_CurFB.Get(), m_CurFrmIdx);
	if (m_Playing)
	{
		m_AccTime += EngineDT;
	}

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("PLAY", ImVec2(95.f, 18.f)))
	{
		m_Playing = true;
	}

	ImGui::SameLine((ImGui::GetWindowSize().x * 0.5f) + 5.f);
	if (ImGui::Button("STOP", ImVec2(95.f, 18.f)))
	{
		m_Playing = false;
		m_CurFrmIdx = 0;
		m_AccTime = 0;
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Current Frame Index / Sprite
	ImGui::Text("Current Frame");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(30.f);
	ImGui::InputInt("##FlipBookFrmIdx", (int*)&m_CurFrmIdx, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::SameLine(0.f, 5.f);
	ImGui::SetNextItemWidth(150.f);
	path Path = m_CurSprite.Get()->GetKey();
	string spriteKey = Path.stem().string();
	ImGui::InputText("##SpriteKey", spriteKey.data(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::SameLine(0.f, 5.f);
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{ 
		if (m_CurFrmIdx == 0)
			m_CurFrmIdx = 0;
		else
			m_CurFrmIdx--;
	}
	ImGui::SameLine(0.0f, 5.f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		if (m_CurFrmIdx >= m_CurFB->GetMaxFrameCount() - 1)
			m_CurFrmIdx = m_CurFB->GetMaxFrameCount() - 1;
		else
			m_CurFrmIdx++;
	}
	ImGui::PopButtonRepeat();
	m_UIHeight += (int)ImGui::GetItemRectSize().y;


	// FPS
	ImGui::Text("FPS");
	float FPS = m_CurFB->GetFPS();
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(120);
	ImGui::InputFloat("##FlipBookFPS", &FPS, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Repeat
	ImGui::Text("Repeat");
	ImGui::SameLine(120);
	if (ImGui::Checkbox("##FlipBookRepeat", &m_Repeat))
	{
		m_FBCom->SetRepeat(m_Repeat);
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	SetChildSize(ImVec2(0.f, (float)m_UIHeight));
}

void FlipBookComUI::SetCurFlipBookInfo()
{
	m_CurFB = m_FBCom->GetVecFlipBook()[m_ListIdx];

	if (m_CurFB.Get() != nullptr)
	{
		m_CurSprite = m_CurFB->GetSprite(m_CurFrmIdx);
		m_Repeat = m_FBCom->GetRepeat();
	}
}

void FlipBookComUI::FlipBookList()
{
	vector<Ptr<CFlipBook>> vFB = m_FBCom->GetVecFlipBook();

	ImGui::SeparatorText("FlipBook List");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (ImGui::BeginListBox("##FE_AddedSpriteList", ImVec2(-FLT_MIN, 150.f)))
	{
		for (size_t i = 0; i < vFB.size(); i++)
		{
			const bool is_selected = (m_ListIdx == i);
			ImGui::BeginGroup();
			// Button
			string index = std::to_string(i);
			ImGui::BeginDisabled();
			ImGui::Button(index.c_str(), ImVec2(18.f, 18.f));
			ImGui::EndDisabled();
			// Label
			string label;
			if (vFB[i] == nullptr)
			{
				label = "None";
			}
			else
			{
				path Path = vFB[i]->GetKey();
				label = Path.stem().string();
			}
			
			ImGui::SameLine(0.f, 10.f);
			if (ImGui::Selectable(label.c_str(), is_selected))
			{
				m_ListIdx = i;
				//GetDetail()->SetCurSpriteIndex(i);
			}
			ImGui::EndGroup();

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				m_ListIdx = i;
				SetCurFlipBookInfo();
			}
		}
		ImGui::EndListBox();
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void FlipBookComUI::EditFlipBookList()
{
	ImGui::SetCursorPosX(ImGui::GetWindowWidth() * 0.5f - 105.f);
	if (ImGui::Button("Add", ImVec2(100.f, 18.f)))
	{
		m_AddMode = true;
	}

	ImGui::SameLine(0.f, 10.f);
	if (ImGui::Button("Delete", ImVec2(100.f, 18.f)))
	{
		m_FBCom->EraseFlipBook(m_ListIdx);
		m_ListIdx--;

		if (m_ListIdx - 1 < 0)
			m_ListIdx = 0;

		SetCurFlipBookInfo();
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void FlipBookComUI::CurFlipBookInfo()
{
	ImGui::SeparatorText("Current FlipBook Information");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::Text("FlipBook Index");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(50);
	ImGui::InputInt("##FlipBookIndex", &m_ListIdx, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(0.f, 5.f);
	if (ImGui::ArrowButton("##FlipBookIndexL", ImGuiDir_Left))
	{
		m_ListIdx--;
		if (m_ListIdx - 1 < 0)
			m_ListIdx = 0;

		m_CurFB = m_FBCom->GetVecFlipBook().at(m_ListIdx);
	}
	ImGui::SameLine(0.f, 5.f);
	if (ImGui::ArrowButton("##FlipBookIndexR", ImGuiDir_Right))
	{
		m_ListIdx++;
		if (m_ListIdx > m_FBCom->GetVecFlipBook().size() - 1)
			m_ListIdx = m_FBCom->GetVecFlipBook().size() - 1;

		m_CurFB = m_FBCom->GetVecFlipBook().at(m_ListIdx);
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::Text("FlipBook Name");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(230.f);
	string Key;
	if (m_CurFB.Get() == nullptr)
		Key = "None";
	else
		Key = string(m_CurFB.Get()->GetKey().begin(), m_CurFB.Get()->GetKey().end());
	ImGui::InputText("##FlipBookKey", (char*)Key.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void FlipBookComUI::SelectFlipBook(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_SelectFB = nullptr;
		return;
	}
	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(strAssetName);

	assert(pFlipBook.Get());

	m_SelectFB = pFlipBook;
}

void FlipBookComUI::AddFlipBookPopup()
{
	//ImGui::OpenPopup("Add FlipBook");

	ImVec2 pos = ImGui::GetWindowPos();
	ImVec2 center = ImVec2(pos.x + (ImGui::GetWindowWidth() / 2.f), pos.y + (ImGui::GetWindowHeight() / 3.f));
	ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

	string name = "";

	if (m_SelectFB != nullptr)
		name = string(m_SelectFB->GetKey().begin(), m_SelectFB->GetKey().end());

	if (ImGui::Begin("Add FlipBook", &m_AddMode, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize))
	//if (ImGui::BeginPopup("Add FlipBook", NULL))
	{
		ImGui::Text("Select The Index And FlipBook To Add");
		ImGui::Text("");
		ImGui::Separator();
		ImGui::Text("FlipBook");
		ImGui::SameLine(120.f);
		ImGui::InputText("##AddFlipBookName", name.data(), ImGuiInputTextFlags_ReadOnly);

		ImGui::SameLine();
		if (ImGui::Button("##FlipBookBtn", ImVec2(18.f, 18.f)))
		{
			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
			pListUI->SetShowNameOnly(true);
			pListUI->SetName("FlipBook");
		
			vector<string> vecFlipBookNames;
			const map<wstring, Ptr<CAsset>>& mapFlipBook = CAssetMgr::GetInst()->GetAssets(ASSET_TYPE::FLIPBOOK);

			for (const auto& pair : mapFlipBook)
			{
				vecFlipBookNames.push_back(string(pair.first.begin(), pair.first.end()));
			}
		
			pListUI->AddList(vecFlipBookNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&FlipBookComUI::SelectFlipBook);
			pListUI->SetActive(true);
		}
		ImGui::Text("Index");
		ImGui::SameLine(120.f);
		ImGui::SetNextItemWidth(80.f);
		ImGui::InputInt("##AddFlipBookIndex", &m_ListIdx, 0.f);

		if (ImGui::Button("Add", ImVec2(120, 0)))
		{
			m_FBCom->AddFlipBook(m_ListIdx, m_SelectFB);
			m_SelectFB = nullptr;
			m_AddMode = false;
			//ImGui::End();
			ImGui::CloseCurrentPopup();
		}
		
		ImGui::SameLine();
		if (ImGui::Button("Cancel", ImVec2(120, 0)))
		{
			m_AddMode = false;
			//ImGui::End();
			ImGui::CloseCurrentPopup();
		}
		//ImGui::EndPopup();
		ImGui::End();
	}
}

void FlipBookComUI::ShowFlipBookSprite(Ptr<CFlipBook> _CurFlipBook, int _CurIndex)
{
	float MaxTime = 1.f / m_CurFB->GetFPS();
	if (MaxTime < m_AccTime)
	{
		m_AccTime -= MaxTime;
		++m_CurFrmIdx;

		if (m_CurFB->GetMaxFrameCount() <= m_CurFrmIdx)
		{
			m_CurFrmIdx = 0;
		}
	}

	m_CurSprite = m_CurFB->GetSprite(m_CurFrmIdx);
	Ptr<CTexture> pTexture = m_CurSprite->GetAtlasTexture();
	
	// Image Size
	ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * pTexture->Width()
				, m_CurSprite->GetBackgroundUV().y * pTexture->Height());

	ImVec2 frameSize = ImVec2(200.f, 200.f);
	if (frameSize.x < crop.x || frameSize.y < crop.y)
	{
		float ratio = crop.x / frameSize.x;
		ratio = 1 / ratio;

		crop.x *= ratio;
		crop.y *= ratio;
	}

	ImVec2 uv_min = ImVec2(m_CurSprite->GetLeftTopUV().x, m_CurSprite->GetLeftTopUV().y);
	ImVec2 uv_max = ImVec2(m_CurSprite->GetLeftTopUV().x + m_CurSprite->GetSliceUV().x
							, m_CurSprite-> GetLeftTopUV().y + m_CurSprite->GetSliceUV().y);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
	ImGui::Image(pTexture->GetSRV().Get(), crop, uv_min, uv_max, tint_col, border_col);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}
