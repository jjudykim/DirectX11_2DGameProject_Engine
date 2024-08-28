#include "pch.h"
#include "MeshRenderUI.h"

#include <Engine/CAssetMgr.h>
#include <Engine/CMeshRender.h>
#include <Engine/CMesh.h>
#include <Engine/CMaterial.h>

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"
#include "ParamUI.h"

MeshRenderUI::MeshRenderUI()
	: ComponentUI(COMPONENT_TYPE::MESHRENDER)
{
}

MeshRenderUI::~MeshRenderUI()
{
}

void MeshRenderUI::Init()
{
	m_MeshRender = GetTargetObject()->MeshRender();
	m_UseFlipBook = GetTargetObject()->GetComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT) != nullptr;
	m_UseSprite = m_MeshRender->GetUseSpriteAsTex();
	m_ParamIdx = m_MeshRender->GetParamIndex();
}

void MeshRenderUI::Update()
{
	if (GetTargetObject() == nullptr || GetTargetObject()->IsDead())
		return;

	if (m_MeshRender == nullptr)
		Init();

	if (m_MeshRender->GetID() != GetTargetObject()->MeshRender()->GetID())
		Init();

	m_UIHeight = 0;

	Title();
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Mesh 정보
	Ptr<CMesh> pMesh = m_MeshRender->GetMesh();

	string MeshName;

	if (pMesh.Get())
		MeshName = string(pMesh->GetKey().begin(), pMesh->GetKey().end());

	ImGui::Text("Mesh");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MeshKey", (char*)MeshName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
	
	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MESH == pAsset->GetAssetType())
			{
				m_MeshRender->SetMesh((CMesh*)pAsset.Get());
			}
		}
		ImGui::EndDragDropTarget();
	}
	
	ImGui::SameLine();
	if (ImGui::Button("##MeshBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("Mesh");
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MESH, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMesh);
		pListUI->SetActive(true);
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Material 정보
	Ptr<CMaterial> pMtrl = m_MeshRender->GetMaterial();

	string MtrlName;
	if (pMtrl.Get())
		MtrlName = string(pMtrl->GetKey().begin(), pMtrl->GetKey().end());

	ImGui::Text("Material");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(150.f);
	ImGui::InputText("##MaterialKey", (char*)MtrlName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::MATERIAL == pAsset->GetAssetType())
			{
				m_MeshRender->SetMaterial((CMaterial*)pAsset.Get());
			}
		}
		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##MtrlBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("Material");
		vector<string> vecMtrlNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::MATERIAL, vecMtrlNames);
		pListUI->AddList(vecMtrlNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::SelectMaterial);
		pListUI->SetActive(true);
		m_UseSprite = false;
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (m_UseFlipBook)
	{
		m_UIHeight += 30.f;
		SetChildSize(ImVec2(0.f, (float)m_UIHeight));
		return;
	}

	// Use Sprite As Texture
	ImGui::Text("Use Sprite as Texture?");
	m_UseSprite = m_MeshRender->GetUseSpriteAsTex();
	ImGui::SameLine(0.f, 10.f);
	ImGui::Checkbox("##UseSprite", &m_UseSprite);
	m_MeshRender->SetUseSpriteAsTex(m_UseSprite);
	if (m_MeshRender->GetMaterial() != nullptr)
		m_MeshRender->GetMaterial()->SetUseSpriteAsTex(m_UseSprite);

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Shader Parameter
	ShaderParameter();

	m_UIHeight += 100.f;
	SetChildSize(ImVec2(0.f, (float)m_UIHeight));
}

void MeshRenderUI::SelectMesh(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_MeshRender->SetMesh(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CMesh> pMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(strAssetName);

	assert(pMesh.Get());

	m_MeshRender->SetMesh(pMesh);
}

void MeshRenderUI::SelectMaterial(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_MeshRender->SetMaterial(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());
	Ptr<CMaterial> pMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(strAssetName);

	assert(pMtrl.Get());

	m_MeshRender->SetMaterial(pMtrl);
}

void MeshRenderUI::ShaderParameter()
{
	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
	ImGui::SeparatorText("Shader Parameter");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	Ptr<CMaterial> pMtrl = m_MeshRender->GetMaterial();

	if (pMtrl == nullptr)
		return;

	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	// Shader가 요구하는 파라미터 목록을 가져옴
	const vector<tScalarParam>& vecScalarParam = pShader->GetScalarParam();
	const vector<tTexParam>& vecTexParam = pShader->GetTexParam();

	// Scalar Parameter 대응
	for (size_t i = 0; i < vecScalarParam.size(); ++i)
	{
		switch (vecScalarParam[i].ParamType)
		{
		case INT_0:
		case INT_1:
		case INT_2:
		case INT_3:
		{
			int data = m_MeshRender->GetParamIndex();
			if (ParamUI::InputInt(&data, vecScalarParam[i].strDesc))
			{
				m_MeshRender->SetParamIdx(data);
				m_UIHeight += (int)ImGui::GetItemRectSize().y;
			}
		}
		break;
		case FLOAT_0:
		case FLOAT_1:
		case FLOAT_2:
		case FLOAT_3:
		{
			float data = *((float*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragFloat(&data, 0.1f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
				m_UIHeight += (int)ImGui::GetItemRectSize().y;
			}
		}
		break;
		case VEC2_0:
		case VEC2_1:
		case VEC2_2:
		case VEC2_3:
		{
			Vec2 data = *((Vec2*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragVec2(&data, 0.1f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
				m_UIHeight += (int)ImGui::GetItemRectSize().y;
			}
		}
		break;
		case VEC4_0:
		case VEC4_1:
		case VEC4_2:
		case VEC4_3:
		{
			Vec4 data = *((Vec4*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragVec4(&data, 0.1f, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
				m_UIHeight += (int)ImGui::GetItemRectSize().y;
			}
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			Matrix data = *((Matrix*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
		}
		break;
		}
	}

	if (ImGui::BeginListBox("##TexParamList", ImVec2(-FLT_MIN, 150.f)))
	{
		for (size_t i = 0; i < vecTexParam.size(); i++)
		{
			const bool is_selected = (m_ParamIdx == i);
			ImGui::BeginGroup();
			// Button
			string index = std::to_string(i);
			ImGui::BeginDisabled();
			ImGui::Button(index.c_str(), ImVec2(18.f, 18.f));
			ImGui::EndDisabled();
			// Label
			string label;
			label = vecTexParam[i].strDesc;

			ImGui::SameLine(0.f, 10.f);
			if (ImGui::Selectable(label.c_str(), is_selected))
			{
				m_ParamIdx = i;
				//GetDetail()->SetCurSpriteIndex(i);
			}
			ImGui::EndGroup();

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				m_ParamIdx = i;
			}
		}
		ImGui::EndListBox();
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	// Texture Parameter 대응
	if (!m_UseSprite)
	{
		Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[m_ParamIdx].ParamType);

		if (ParamUI::InputTexture(pCurTex, vecTexParam[m_ParamIdx].strDesc, this, (DELEGATE_1)&MeshRenderUI::ChangeTexture))
		{
			pMtrl->SetTexParam(vecTexParam[m_ParamIdx].ParamType, pCurTex);
			m_SelectTexParam = vecTexParam[m_ParamIdx].ParamType;
		}
	}
	else
	{
		Ptr<CSprite> CurSprite = pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType);

		ImVec2 frameSize = ImVec2(200.f, 200.f);
		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
		ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);

		if (CurSprite != nullptr)
		{
			Ptr<CTexture> CurTex = CurTex = CurSprite->GetAtlasTexture();

			string Name = string(CurSprite->GetName().begin(), CurSprite->GetName().end());

			ImGui::Text(Name.c_str());
			ImGui::SameLine(120);

			ImVec2 crop = ImVec2(CurSprite->GetBackgroundUV().x * CurTex->Width()
				, CurSprite->GetBackgroundUV().y * CurTex->Height());

			if (frameSize.x < crop.x || frameSize.y < crop.y)
			{
				float ratio = crop.x / frameSize.x;
				ratio = 1 / ratio;

				crop.x *= ratio;
				crop.y *= ratio;
			}

			ImVec2 uv_min = ImVec2(CurSprite->GetLeftTopUV().x, CurSprite->GetLeftTopUV().y);
			ImVec2 uv_max = ImVec2(CurSprite->GetLeftTopUV().x + CurSprite->GetSliceUV().x
				, CurSprite->GetLeftTopUV().y + CurSprite->GetSliceUV().y);

			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
			ImGui::Image(CurTex->GetSRV().Get(), crop, uv_min, uv_max, tint_col, border_col);
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
		}
		else
		{
			ImGui::Image(nullptr, frameSize, ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), tint_col, border_col);
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
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
					CurSprite = ((CSprite*)pAsset.Get());
				}
			}
			ImGui::EndDragDropTarget();
		}

		// DragDrop으로 원본 텍스쳐가 바뀐 경우
		if (CurSprite != pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType))
		{
			pMtrl->SetSprite(vecTexParam[m_ParamIdx].ParamType, CurSprite);
			return;
		}

		// List Button
		if (this == nullptr && (DELEGATE_1)&MeshRenderUI::ChangeSprite == nullptr)
			return;

		ImGui::SameLine();
		if (ImGui::Button("##SpriteChangeButton", ImVec2(18.f, 18.f)))
		{
			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
			pListUI->SetName("Sprite");

			vector<string> vecSpriteNames;
			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
			pListUI->AddList(vecSpriteNames);
			pListUI->AddDelegate(this, (DELEGATE_1)&MeshRenderUI::ChangeSprite);
			pListUI->SetActive(true);
		}
	}

	if (ImGui::Button("Apply", ImVec2(30.f, 18.f)))
	{
		m_SelectTexParam = (TEX_PARAM)m_ParamIdx;
	}
}

void MeshRenderUI::ChangeTexture(DWORD_PTR Param)
{
	// Texture Parameter를 입력받을 Material
	Ptr<CMaterial> pMtrl = m_MeshRender->GetMaterial();

	ListUI* pListUI = (ListUI*)Param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMtrl->SetTexParam(m_SelectTexParam, nullptr);

		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pMtrl.Get());

	pMtrl->SetTexParam(m_SelectTexParam, pTex);
}

void MeshRenderUI::ChangeSprite(DWORD_PTR Param)
{
	Ptr<CMaterial> pMtrl = m_MeshRender->GetMaterial();

	ListUI* pListUI = (ListUI*)Param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMtrl->SetSprite(m_SelectTexParam, nullptr);
		
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pMtrl.Get());

	pMtrl->SetSprite(m_SelectTexParam, pSprite);
}
