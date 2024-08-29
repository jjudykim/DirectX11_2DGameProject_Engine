#include "pch.h"
#include "Inspector.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include "ComponentUI.h"
#include "AssetUI.h"
#include "ScriptUI.h"

Inspector::Inspector()
	: m_TargetObject(nullptr)
	, m_arrComUI{}
	, m_arrAssetUI{}
{
}

Inspector::~Inspector()
{
}

void Inspector::Update()
{
	if (m_TargetObject == nullptr || m_TargetObject->IsDead())
	{
		return;
	}

	SetTargetObject(m_TargetObject);

	// ===========
	// Object Name
	// ===========

	char Name[256];
	string strObjectName = string(m_TargetObject->GetName().begin(), m_TargetObject->GetName().end());
	strcpy_s(Name, strObjectName.c_str());

	ImGui::Text("ObjectName");
	ImGui::SameLine(108);
	ImGui::InputText("##ObjectName", Name, 256);
	strObjectName = Name;
	m_TargetObject->SetName(wstring(strObjectName.begin(), strObjectName.end()));

	// ===========
	//    Layer
	// ===========
	int LayerIdx = m_TargetObject->GetLayerIdx();
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	CLayer* pLayer = pCurLevel->GetLayer(LayerIdx);
	int selectItem = LayerIdx;

	ImGui::Text("Layer");
	ImGui::SameLine(108);
	ImGui::Combo("##LayerCombo", &selectItem, LAYER_TYPE_STRING, IM_ARRAYSIZE(LAYER_TYPE_STRING));
	if (selectItem != LayerIdx)
	{
		pCurLevel->AddObject(selectItem, m_TargetObject, false);
	}
}

void Inspector::SetTargetObject(CGameObject* _Object)
{
	m_TargetObject = _Object;

	// Object가 보유하고 있는 Component에 대응하는 ComponentUI가 활성화
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (m_arrComUI[i] == nullptr)
			continue;

		m_arrComUI[i]->SetTargetObject(_Object);
	}

	// Object가 보유하고 있는 Script마다 ScriptUI가 활성화
	if (m_TargetObject == nullptr)
	{
		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}
	else
	{
		const vector<CScript*>& vecScripts = m_TargetObject->GetScripts();

		// ScriptUI Count Check
		if (m_vecScriptUI.size() < vecScripts.size())
		{
			CreateScriptUI((UINT)(vecScripts.size() - m_vecScriptUI.size()));
		}

		for (size_t i = 0; i < m_vecScriptUI.size(); ++i)
		{
			if (i < vecScripts.size())
				m_vecScriptUI[i]->SetTargetScript(vecScripts[i]);
			else
				m_vecScriptUI[i]->SetTargetScript(nullptr);
		}
	}


	// Asset 비활성화
	m_TargetAsset = nullptr;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(nullptr);
	}
}

void Inspector::SetTargetAsset(Ptr<CAsset> _Asset)
{
	if (_Asset == nullptr)
		return;

	SetTargetObject(nullptr);

	m_TargetAsset = _Asset;

	for (UINT i = 0; i < (UINT)ASSET_TYPE::END; ++i)
	{
		m_arrAssetUI[i]->SetAsset(m_TargetAsset);
	}
}