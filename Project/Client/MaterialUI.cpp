#include "pch.h"
#include "MaterialUI.h"

#include "ListUI.h"
#include "TreeUI.h"
#include "CEditorMgr.h"
#include "ParamUI.h"

#include <Engine/CAssetMgr.h>


MaterialUI::MaterialUI()
	: AssetUI(ASSET_TYPE::MATERIAL)
{
}

MaterialUI::~MaterialUI()
{
}

void MaterialUI::Update()
{
	Title();

	// Material Name
	OutputAssetName();

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	// 참조하는 Shader
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();
	string Name;

	if (pShader == nullptr)
		Name = "None";
	else
		Name = string(pShader->GetKey().begin(), pShader->GetKey().end());

	// Shader Name
	ImGui::Text("Shader Name");
	ImGui::SameLine(100);
	ImGui::InputText("##ShaderName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::GRAPHIC_SHADER == pAsset->GetAssetType())
			{
				pMtrl->SetShader((CGraphicShader*)pAsset.Get());
				SaveMaterialToFile();
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##ShaderBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("GraphicShader");
		vector<string> vecMeshNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::GRAPHIC_SHADER, vecMeshNames);
		pListUI->AddList(vecMeshNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::SelectShader);
		pListUI->SetActive(true);
	}

	// Use Sprite As Texture
	ImGui::Text("Use Sprite as Texture?");
	m_UseSprite = pMtrl->GetUseSpriteAsTex();
	ImGui::SameLine(0.f, 10.f);
	ImGui::Checkbox("##UseSprite", &m_UseSprite);
	pMtrl->SetUseSpriteAsTex(m_UseSprite);

	// Shader Parameter
	ShaderParameter();

	if (ImGui::Button("Save", ImVec2(30.f, 18.f)))
	{
		pMtrl->Save(pMtrl->GetKey());
	}
}

void MaterialUI::ShaderParameter()
{
	ImGui::SeparatorText("Shader Parameter");

	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();
	Ptr<CGraphicShader> pShader = pMtrl->GetShader();

	if (pShader == nullptr)
		return;

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
			int data = *((int*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
			if (ParamUI::DragInt(&data, 1, vecScalarParam[i].strDesc))
			{
				pMtrl->SetScalarParam(vecScalarParam[i].ParamType, data);
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
			}
		}
		break;
		case MAT_0:
		case MAT_1:
		case MAT_2:
		case MAT_3:
		{
			Matrix data = *((Matrix*)pMtrl->GetScalarParam(vecScalarParam[i].ParamType));
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

	
	if (vecTexParam.size() == 1)
	{
		Ptr<CTexture>pCurTex = pMtrl->GetTexParam(vecTexParam[0].ParamType);

		if (ParamUI::InputTexture(pCurTex, vecTexParam[0].strDesc, this, (DELEGATE_1)&MaterialUI::ChangeTexture))
		{
			pMtrl->SetTexParam(vecTexParam[0].ParamType, pCurTex);
			m_SelectTexParam = vecTexParam[0].ParamType;
		}
	}
	else
	{
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

		// Texture Parameter 대응
		if (!m_UseSprite)
		{
			Ptr<CTexture> pCurTex = nullptr;

			if (vecTexParam.size() > 0)
				pCurTex = pMtrl->GetTexParam(vecTexParam[m_ParamIdx].ParamType);

			if (pCurTex != nullptr)
			{
				if (ParamUI::InputTexture(pCurTex, vecTexParam[m_ParamIdx].strDesc, this, (DELEGATE_1)&MaterialUI::ChangeTexture))
				{
					pMtrl->SetTexParam(vecTexParam[m_ParamIdx].ParamType, pCurTex);
					m_SelectTexParam = vecTexParam[m_ParamIdx].ParamType;
				}
			}
		}
		else
		{
			Ptr<CSprite> CurSprite = nullptr;

			if (vecTexParam.size() > 0)
				CurSprite = pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType);

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
			}
			else
			{
				ImGui::Image(nullptr, frameSize, ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), tint_col, border_col);
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
			if (vecTexParam.size() > 0)
			{
				if (CurSprite != pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType))
				{
					pMtrl->SetSprite(vecTexParam[m_ParamIdx].ParamType, CurSprite);
					return;
				}
			}

			// List Button
			if (this == nullptr && (DELEGATE_1)&MaterialUI::ChangeSprite == nullptr)
				return;

			ImGui::SameLine();
			if (ImGui::Button("##SpriteChangeButton", ImVec2(18.f, 18.f)))
			{
				ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
				pListUI->SetName("Sprite");

				vector<string> vecSpriteNames;
				CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
				pListUI->AddList(vecSpriteNames);
				pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::ChangeSprite);
				pListUI->SetActive(true);
			}
		}
	}

	//// Texture Parameter 대응
	//if (!m_UseSprite)
	//{
	//	if (vecTexParam.size() > 0)
	//	{
	//		Ptr<CTexture> pCurTex = pMtrl->GetTexParam(vecTexParam[m_ParamIdx].ParamType);
	//
	//		// Current Asset
	//		string TexKey;
	//		if (pCurTex == nullptr) TexKey = "";
	//		else TexKey = string(pCurTex->GetKey().begin(), pCurTex->GetKey().end());
	//		ImGui::Text("Setted Asset");
	//		ImGui::SameLine(0.f, 10.f);
	//		ImGui::SetNextItemWidth(250.f);
	//		ImGui::InputText("##SettedAssetKey", TexKey.data(), ImGuiInputTextFlags_ReadOnly);
	//
	//		if (ParamUI::InputTexture(pCurTex, vecTexParam[m_ParamIdx].strDesc, this, (DELEGATE_1)&MaterialUI::ChangeTexture))
	//		{
	//			pMtrl->SetTexParam(vecTexParam[m_ParamIdx].ParamType, pCurTex);
	//			m_SelectTexParam = vecTexParam[m_ParamIdx].ParamType;
	//			SaveMaterialToFile();
	//		}
	//	}
	//}
	//else
	//{
	//	if (vecTexParam.size() > 0)
	//	{
	//		Ptr<CSprite> pCurSprite = pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType);
	//
	//		// Current Asset
	//		string TexKey;
	//		if (pCurSprite == nullptr) TexKey = "";
	//		else TexKey = string(pCurSprite->GetKey().begin(), pCurSprite->GetKey().end());
	//		ImGui::Text("Setted Asset");
	//		ImGui::SameLine(0.f, 10.f);
	//		ImGui::SetNextItemWidth(250.f);
	//		ImGui::InputText("##SettedAssetKey", TexKey.data(), ImGuiInputTextFlags_ReadOnly);
	//
	//		ImVec2 frameSize = ImVec2(200.f, 200.f);
	//		ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	//		ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);
	//
	//		if (pCurSprite != nullptr)
	//		{
	//			Ptr<CTexture> CurTex = pCurSprite->GetAtlasTexture();
	//
	//			string Name = string(pCurSprite->GetName().begin(), pCurSprite->GetName().end());
	//
	//			ImGui::Text(Name.c_str());
	//			ImGui::SameLine(120);
	//
	//			ImVec2 crop = ImVec2(pCurSprite->GetBackgroundUV().x * CurTex->Width()
	//				, pCurSprite->GetBackgroundUV().y * CurTex->Height());
	//
	//			if (frameSize.x < crop.x || frameSize.y < crop.y)
	//			{
	//				float ratio = crop.x / frameSize.x;
	//				ratio = 1 / ratio;
	//
	//				crop.x *= ratio;
	//				crop.y *= ratio;
	//			}
	//
	//			ImVec2 uv_min = ImVec2(pCurSprite->GetLeftTopUV().x, pCurSprite->GetLeftTopUV().y);
	//			ImVec2 uv_max = ImVec2(pCurSprite->GetLeftTopUV().x + pCurSprite->GetSliceUV().x
	//				, pCurSprite->GetLeftTopUV().y + pCurSprite->GetSliceUV().y);
	//
	//			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
	//			ImGui::Image(CurTex->GetSRV().Get(), crop, uv_min, uv_max, tint_col, border_col);
	//		}
	//		else
	//		{
	//			ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
	//			ImGui::Image(nullptr, frameSize, ImVec2(0.f, 0.f), ImVec2(1.f, 1.f), tint_col, border_col);
	//		}
	//
	//		if (ImGui::BeginDragDropTarget())
	//		{
	//			const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");
	//			if (payload)
	//			{
	//				TreeNode** ppNode = (TreeNode**)payload->Data;
	//				TreeNode* pNode = *ppNode;
	//
	//				Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
	//				if (ASSET_TYPE::SPRITE == pAsset->GetAssetType())
	//				{
	//					pCurSprite = ((CSprite*)pAsset.Get());
	//				}
	//			}
	//			ImGui::EndDragDropTarget();
	//		}
	//
	//		// DragDrop으로 원본 텍스쳐가 바뀐 경우
	//		if (pCurSprite != pMtrl->GetSprite(vecTexParam[m_ParamIdx].ParamType))
	//		{
	//			pMtrl->SetSprite(vecTexParam[m_ParamIdx].ParamType, pCurSprite);
	//			SaveMaterialToFile();
	//			return;
	//		}
	//
	//		// List Button
	//		if (this == nullptr && (DELEGATE_1)&MaterialUI::ChangeSprite == nullptr)
	//			return;
	//
	//		ImGui::SameLine();
	//		if (ImGui::Button("##SpriteChangeButton", ImVec2(18.f, 18.f)))
	//		{
	//			ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
	//			pListUI->SetName("Sprite");
	//
	//			vector<string> vecSpriteNames;
	//			CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::SPRITE, vecSpriteNames);
	//			pListUI->AddList(vecSpriteNames);
	//			pListUI->AddDelegate(this, (DELEGATE_1)&MaterialUI::ChangeSprite);
	//			pListUI->SetActive(true);
	//		}
	//	}
	//}
}

void MaterialUI::SelectShader(DWORD_PTR _ListUI)
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMtrl->SetShader(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CGraphicShader> pShader = CAssetMgr::GetInst()->FindAsset<CGraphicShader>(strAssetName);

	assert(pShader.Get());

	pMtrl->SetShader(pShader);

	SaveMaterialToFile();
}

void MaterialUI::ChangeTexture(DWORD_PTR Param)
{
	// Texture Parameter를 입력받을 Material
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	ListUI* pListUI = (ListUI*)Param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMtrl->SetTexParam(m_SelectTexParam, nullptr);
		SaveMaterialToFile();
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);
	
	assert(pMtrl.Get());

	pMtrl->SetTexParam(m_SelectTexParam, pTex);
	SaveMaterialToFile();
}

void MaterialUI::ChangeSprite(DWORD_PTR param)
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	ListUI* pListUI = (ListUI*)param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		pMtrl->SetSprite((TEX_PARAM)m_ParamIdx, nullptr);
		SaveMaterialToFile();
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CSprite> pSprite = CAssetMgr::GetInst()->FindAsset<CSprite>(strAssetName);

	assert(pMtrl.Get());

	pMtrl->SetSprite((TEX_PARAM)m_ParamIdx, pSprite);
	SaveMaterialToFile();
}

void MaterialUI::SaveMaterialToFile()
{
	Ptr<CMaterial> pMtrl = (CMaterial*)GetAsset().Get();

	if (!pMtrl->IsEngineAsset())
	{
		if (FAILED(pMtrl->Save(pMtrl->GetRelativePath())))
		{
			int a = 0;
		}
	}
}
