#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>
#include <Engine/CCollisionMgr.h>
#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>

Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
	, m_ColliderCom(nullptr)
	, m_Scale(ImVec4(0.f, 0.f, 0.f, 0.f))
	, m_Offset(ImVec4(0.f, 0.f, 0.f, 0.f))
	, m_Independent(false)
	, m_Ratio(100)
{
}

Collider2DUI::~Collider2DUI()
{
}

void Collider2DUI::Init()
{
	m_ColliderCom = GetTargetObject()->Collider2D();

	m_Scale = ImVec4(m_ColliderCom->GetScale().x, m_ColliderCom->GetScale().y, m_ColliderCom->GetScale().z, 0.f);
	m_Offset = ImVec4(m_ColliderCom->GetOffset().x, m_ColliderCom->GetOffset().y, m_ColliderCom->GetOffset().z, 0.f);

	m_Independent = m_ColliderCom->GetIndependentScale();
}

void Collider2DUI::Update()
{
	if (GetTargetObject() == nullptr || GetTargetObject()->IsDead())
		return;

	if (m_ColliderCom == nullptr || m_ColliderCom->GetID() != GetTargetObject()->Collider2D()->GetID())
		Init();

	Title();

	// Scale
	float scale[3] = { m_Scale.x * m_Ratio, m_Scale.y * m_Ratio, m_Scale.z * m_Ratio };
	ImGui::Text("Scale");
	ImGui::SameLine(120.f);
	if (m_Independent)
		ImGui::DragFloat3("##Collider2DScale", scale, 1.f);
	else ImGui::DragFloat3("##Collider2DScale", scale, 0.1f);

	m_Scale.x = scale[0] / m_Ratio;
	m_Scale.y = scale[1] / m_Ratio;
	m_Scale.z = scale[2] / m_Ratio;
	
	m_ColliderCom->SetScale(Vec3(m_Scale.x, m_Scale.y, m_Scale.z));

	// Offset Pos
	float offset[3] = { m_Offset.x * m_Ratio, m_Offset.y * m_Ratio, m_Offset.z * m_Ratio };
	ImGui::Text("Offset");
	ImGui::SameLine(120.f);
	if (m_Independent)
		ImGui::DragFloat3("##Collider2DOffset", offset, 1.f);
	else ImGui::DragFloat3("##Collider2DOffset", offset, 0.1f);

	m_Offset.x = offset[0] / m_Ratio;
	m_Offset.y = offset[1] / m_Ratio;
	m_Offset.z = offset[2] / m_Ratio;

	m_ColliderCom->SetOffset(Vec3(m_Offset.x, m_Offset.y, m_Offset.z));

	// Independent Scale CheckBox
	ImGui::Text("Independent Of Object Scale");
	ImGui::SameLine(0.f, 10.f);
	ImGui::Checkbox("##Collider2DIndependent", &m_Independent);
	m_ColliderCom->SetIndependentScale(m_Independent);

	// Layer Collision Check
	LayerCollisionCheck();
}

void Collider2DUI::ApplyColliderDetail()
{
	m_ColliderCom->SetScale(Vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	m_ColliderCom->SetOffset(Vec3(m_Offset.x, m_Offset.y, m_Offset.z));
	m_ColliderCom->SetIndependentScale(m_Independent);
}

void Collider2DUI::LayerCollisionCheck()
{
	ImGui::Text("Collision Check");
	ImGui::SameLine(120);
	if (ImGui::Button("Show##LayerCollisionBtn", ImVec2(50.f, 20.f)))
	{
		m_ShowLayerCheck ? m_ShowLayerCheck = false : m_ShowLayerCheck = true;
	}

	if (!m_ShowLayerCheck)
		return;

	int CurLayer = GetTargetObject()->GetLayerIdx();

	wstring CurLayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(CurLayer)->GetName();
	string strCurLayerName = string(CurLayerName.begin(), CurLayerName.end());
	ImGui::Text("Layer %d :", CurLayer);
	ImGui::SameLine(0.f, 5.f);
	ImGui::Text(strCurLayerName.c_str());


	bool bLayer[32] = {};
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		bLayer[i] = CCollisionMgr::GetInst()->IsCollisionChecked(i, CurLayer);
	}

	int ChangedIdx = -1;
	if (ImGui::BeginTable("##CollisionCheckTable", 2, ImGuiTableFlags_Resizable | ImGuiTableFlags_NoSavedSettings | ImGuiTableFlags_Borders))
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			char label[MAX_LAYER];
			sprintf_s(label, "Layer %d", i);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(label, &bLayer[i], ImGuiSelectableFlags_SpanAllColumns))
			{
				ChangedIdx = i;
			}

			wstring wlayerName = CLevelMgr::GetInst()->GetCurrentLevel()->GetLayer(i)->GetName();
			string layerName(wlayerName.begin(), wlayerName.end());
			ImGui::TableNextColumn();
			ImGui::Text(layerName.c_str());
		}
		ImGui::EndTable();
	}

	if (ChangedIdx != -1)
	{
		CCollisionMgr::GetInst()->CollisionCheck(ChangedIdx, CurLayer);
		CLevelMgr::GetInst()->GetCurrentLevel()->SetCollisionInfo(CCollisionMgr::GetInst()->GetCollisionByLevel());
	}
}
