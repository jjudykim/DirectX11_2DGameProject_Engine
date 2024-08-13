#include "pch.h"
#include "SE_Detail.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_AtlasView.h"

#include <Engine/CAsset.h>
#include <Engine/CSprite.h>
#include <Engine/CAssetMgr.h>

SE_Detail::SE_Detail()
	: m_Background(ImVec2(0, 0))
	, m_CurIndex(0)
{
}

SE_Detail::~SE_Detail()
{
}

void SE_Detail::Init()
{
}

void SE_Detail::Update()
{
	Atlas();
	AtlasInfo();
	ImGui::SeparatorText("Select Sprite");
	SelectSpriteInfo();
	ImGui::SeparatorText("Added Sprite List");
	SpriteList();
}

void SE_Detail::Atlas()
{
	string TexName;

	if (m_AtlasTex != nullptr)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Tex");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputText("##AtlasTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetAtlasTex((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##AtlasTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SE_Detail::SelectTexture);
		pListUI->SetActive(true);
	}
}

void SE_Detail::AtlasInfo()
{
	// Resolution
	UINT width = 0;
	UINT height = 0;

	if (m_AtlasTex != nullptr)
	{
		width = m_AtlasTex->Width();
		height = m_AtlasTex->Height();
	}

	char buff[50] = {};
	sprintf_s(buff, "%d", width);
	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureHegiht", buff, 50, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("");
}

void SE_Detail::SelectSpriteInfo()
{
	path Path = m_AtlasTex.Get()->GetKey();
	string str = Path.stem().string();
	str += "_" + std::to_string(m_vecAddSprite.size());
	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::InputText("##SpriteName", (char*)str.c_str(), 255, ImGuiInputTextFlags_None);

	ImVec2 SpriteLT = GetAtlasView()->GetSpriteLT();
	ImVec2 SpriteRB = GetAtlasView()->GetSpriteRB();
	ImVec2 SpriteSize = ImVec2(SpriteRB.x - SpriteLT.x, SpriteRB.y - SpriteLT.y);

	if (m_Background.x < SpriteSize.x || m_Background.y < SpriteSize.y)
	{
		float size = SpriteSize.x < SpriteSize.y ? SpriteSize.y : SpriteSize.x;
		m_Background = ImVec2(size, size);
	}

	if (SpriteSize.x == 0 || SpriteSize.y == 0)
	{
		SpriteLT = ImVec2(0, 0);
		SpriteRB = ImVec2(0, 0);
	}

	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpritePosX", &SpriteLT.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpritePosY", &SpriteLT.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Size");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpriteSizeX", &SpriteSize.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpriteSizeY", &SpriteSize.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##BackgroundX", &m_Background.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##BackgroundY", &m_Background.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

	if (SpriteSize.x != 0 && SpriteSize.y != 0)
	{
		// Create Sprite
		m_CurSprite = new CSprite;
		Vec2 vSpriteLT = { SpriteLT.x, SpriteLT.y };
		Vec2 vSpriteSize = { SpriteSize.x, SpriteSize.y };
		
		m_CurSprite->Create(m_AtlasTex, vSpriteLT, vSpriteSize);
		m_CurSprite->SetName(wstring(str.begin(), str.end()));
		m_CurSprite->SetBackground(Vec2(m_Background.x, m_Background.y));

		// Image size
		ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasTex->Width()
			, m_CurSprite->GetBackgroundUV().y * m_AtlasTex->Height());

		ImVec2 frameSize = ImVec2(200.f, 200.f);
		if (frameSize.x < crop.x || frameSize.y < crop.y)
		{
			float ratio = crop.x / frameSize.x;
			ratio = 1 / ratio;

			crop.x *= ratio;
			crop.y *= ratio;
		}


		// Sprite Image
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - crop.x) * 0.5f);
		ImGui::Image(m_AtlasTex->GetSRV().Get(), crop, ImVec2(m_CurSprite->GetLeftTopUV().x, m_CurSprite->GetLeftTopUV().y)
															 , ImVec2(m_CurSprite->GetLeftTopUV().x + m_CurSprite->GetSliceUV().x
																      , m_CurSprite->GetLeftTopUV().y + m_CurSprite->GetSliceUV().y)
															 , tint_col, border_col);
	}
	

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("Add", ImVec2(100.f, 18.f)))
	{
		if (m_CurSprite != nullptr)
		{
			m_vecAddSprite.push_back(m_CurSprite);
			m_CurSprite = nullptr;
		}
	}
	ImGui::SameLine();
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) + 5.f);
	if (ImGui::Button("Delete", ImVec2(100.f, 18.f)))
	{
	}

	ImGui::Text("");
}

void SE_Detail::SpriteList()
{
	if (ImGui::BeginListBox("##AddedSpriteList", ImVec2(-FLT_MIN, 5 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < m_vecAddSprite.size(); i++)
		{
			const bool is_selected = (m_CurIndex == i);
			string label = string(m_vecAddSprite[i].Get()->GetName().begin(), m_vecAddSprite[i].Get()->GetName().end());
			if (ImGui::Selectable(label.c_str(), is_selected))
				m_CurIndex = i;

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
				ImGui::SetItemDefaultFocus();
		}
		ImGui::EndListBox();
	}
}

void SE_Detail::SetAtlasTex(Ptr<CTexture> _Tex)
{
	m_AtlasTex = _Tex;
	GetAtlasView()->SetAtlasTex(m_AtlasTex);
}

void SE_Detail::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_AtlasTex = nullptr;
		GetAtlasView()->SetAtlasTex(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetAtlasTex(pTex);
}
