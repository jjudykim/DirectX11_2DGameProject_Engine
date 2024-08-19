#include "pch.h"
#include "FE_SpriteList.h"

#include "FE_FBViewer.h"

#include "TreeUI.h"
#include "ListUI.h"
#include "CEditorMgr.h"
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
	m_vecAddedSprite.clear();
	m_ListIndex = 0;
	m_PrevIndex = -1;
	m_IsActive = false;
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
				if (m_PrevIndex != m_ListIndex)
				{
					GetDetail()->SetCurSprite(m_vecAddedSprite[m_ListIndex]);
					GetViewer()->SetCurSprite(m_vecAddedSprite[m_ListIndex]);
					m_PrevIndex = m_ListIndex;
				}
			}
		}
		ImGui::EndListBox();
	}

	if (m_IsActive)
	{
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
					GetDetail()->SetCurSprite(m_vecAddedSprite[m_vecAddedSprite.size() - 1]);
				}
				else
				{
					const char* bundle = reinterpret_cast<const char*>(pNode->GetData());
					if (bundle[0] != '\0')
					{
						const vector<TreeNode*>& vecChildNode = pNode->GetChildNode();

						for (size_t i = 0; i < vecChildNode.size(); ++i)
						{
							pAsset = (CAsset*)vecChildNode[i]->GetData();
							m_vecAddedSprite.push_back((CSprite*)pAsset.Get());
						}
					}
				}
			}
			ImGui::EndDragDropTarget();
		}
	}
	
	ImGui::SameLine(ImGui::GetWindowSize().x - 30.f);
	ImGui::BeginGroup();
	if (ImGui::ArrowButton("##up", ImGuiDir_Up))
	{
		if (m_ListIndex > 0)
		{
			auto temp = m_vecAddedSprite[m_ListIndex - 1];
			m_vecAddedSprite[m_ListIndex - 1] = m_vecAddedSprite[m_ListIndex];
			m_vecAddedSprite[m_ListIndex] = temp;

			m_ListIndex = m_ListIndex - 1;

			GetViewer()->SetCurSprite(m_vecAddedSprite[m_ListIndex]);
		}
	}
	if (ImGui::ArrowButton("##down", ImGuiDir_Down))
	{
		if (m_ListIndex < m_vecAddedSprite.size() - 1 && 0 < m_vecAddedSprite.size())
		{
			auto temp = m_vecAddedSprite[m_ListIndex + 1];
			m_vecAddedSprite[m_ListIndex + 1] = m_vecAddedSprite[m_ListIndex];
			m_vecAddedSprite[m_ListIndex] = temp;

			m_ListIndex = m_ListIndex + 1;

			GetViewer()->SetCurSprite(m_vecAddedSprite[m_ListIndex]);
		}
	}
	ImGui::EndGroup();


	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("Add", ImVec2(100.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetShowNameOnly(true);
		pListUI->SetName("Sprite");

		vector<string> vecSpriteNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
		pListUI->AddList(vecSpriteNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&FE_SpriteList::SelectSprite);
		pListUI->SetActive(true);
	}

	ImGui::SameLine(0.f, 5.f);
	if (ImGui::Button("Delete", ImVec2(100.f, 18.f)))
	{
		vector<Ptr<CSprite>>::iterator iter = m_vecAddedSprite.begin();
		m_vecAddedSprite.erase(iter + m_ListIndex);
	}

	if (!m_IsActive)
		ImGui::EndDisabled();
}

void FE_SpriteList::SelectSprite(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pSprite.Get());

	m_vecAddedSprite.push_back(pSprite);
}

void FE_SpriteList::SetSpriteToList(vector<Ptr<CSprite>>& _vecSprite)
{
	m_vecAddedSprite.clear();

	for (size_t i = 0; i < _vecSprite.size(); ++i)
	{
		m_vecAddedSprite.push_back(_vecSprite[i]);
	}
}


