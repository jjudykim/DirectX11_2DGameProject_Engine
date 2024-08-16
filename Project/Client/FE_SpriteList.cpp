#include "pch.h"
#include "FE_SpriteList.h"

#include "TreeUI.h"
#include "FE_FBDetail.h"

FE_SpriteList::FE_SpriteList()
	: m_IsActive(false)
	, m_ListIndex(0)
{
}

FE_SpriteList::~FE_SpriteList()
{
}

void FE_SpriteList::Init()
{
}

void FE_SpriteList::Update()
{
	if (GetDetail()->GetCurMode()[0] || GetDetail()->GetCurMode()[1])
		m_IsActive = true;
	else
		m_IsActive = false;

	ShowList();
}

void FE_SpriteList::ShowList()
{
	if (!m_IsActive)
		ImGui::BeginDisabled();
	if (ImGui::BeginListBox("##FE_AddedSpriteList", ImVec2(-FLT_MIN - 30.f, 10 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < m_vecAddedSprite.size(); i++)
		{
			const bool is_selected = (m_ListIndex == i);
			path Path = m_vecAddedSprite[i]->GetKey();
			string label = Path.stem().string();
			if (ImGui::Selectable(label.c_str(), is_selected))
			{
				m_ListIndex = i;
				//GetDetail()->SetCurSpriteIndex(i);
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				GetDetail()->SetCurSprite(m_vecAddedSprite[m_ListIndex]);
			}
		}
		ImGui::EndListBox();
	}

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
			{
				m_vecAddedSprite.push_back((CSprite*)pAsset.Get());
			}
		}
		ImGui::EndDragDropTarget();
	}

	
	ImGui::SameLine(ImGui::GetWindowSize().x - 30.f);
	ImGui::BeginGroup();
	if (ImGui::ArrowButton("##up", ImGuiDir_Up))
	{
	}
	if (ImGui::ArrowButton("##down", ImGuiDir_Down))
	{
	}
	ImGui::EndGroup();


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("Add", ImVec2(100.f, 18.f)))
	{
	}

	ImGui::SameLine(0.f, 5.f);
	if (ImGui::Button("Delete", ImVec2(100.f, 18.f)))
	{
	}

	if (!m_IsActive)
		ImGui::EndDisabled();
}


