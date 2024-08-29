#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

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
}

void Collider2DUI::ApplyColliderDetail()
{
	m_ColliderCom->SetScale(Vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	m_ColliderCom->SetOffset(Vec3(m_Offset.x, m_Offset.y, m_Offset.z));
	m_ColliderCom->SetIndependentScale(m_Independent);
}
