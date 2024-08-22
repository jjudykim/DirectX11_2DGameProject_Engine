#include "pch.h"
#include "Collider2DUI.h"

#include <Engine/CCollider2D.h>

Collider2DUI::Collider2DUI()
	: ComponentUI(COMPONENT_TYPE::COLLIDER2D)
	, m_ColliderCom(nullptr)
	, m_Scale(ImVec4(0.f, 0.f, 0.f, 0.f))
	, m_Offset(ImVec4(0.f, 0.f, 0.f, 0.f))
	, m_Independent(false)
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
	if (m_ColliderCom == nullptr)
		Init();

	Title();

	// Offset Scale
	Vec3 ObjectScale = GetTargetObject()->Transform()->GetRelativeScale();

	if (!m_Independent)
		m_Scale = { m_Scale.x * ObjectScale.x, m_Scale.y * ObjectScale.y, m_Scale.z * ObjectScale.z, 0 };

	float scale[3] = { m_Scale.x, m_Scale.y, m_Scale.z };
	ImGui::Text("Scale");
	ImGui::SameLine(120.f);
	ImGui::DragFloat3("##Collider2DScale", scale);

	if (!m_Independent)
	{
		scale[0] /= ObjectScale.x;
		scale[1] /= ObjectScale.y;
		scale[2] /= ObjectScale.z;
	}

	m_Scale.x = scale[0];
	m_Scale.y = scale[1];
	m_Scale.z = scale[2];

	// Offset Pos

	if (!m_Independent)
		m_Offset = { m_Offset.x * ObjectScale.x, m_Offset.y * ObjectScale.y, m_Offset.z * ObjectScale.z, 0 };

	float offset[3] = { m_Offset.x, m_Offset.y, m_Offset.z };
	ImGui::Text("Offset");
	ImGui::SameLine(120.f);
	ImGui::DragFloat3("##Collider2DOffset", offset, 1.f, 0.f, 0.f, "%.2f");

	if (!m_Independent)
	{
		offset[0] /= ObjectScale.x;
		offset[1] /= ObjectScale.y;
		offset[2] /= ObjectScale.z;
	}

	m_Offset.x = offset[0];
	m_Offset.y = offset[1];
	m_Offset.y = offset[2];

	// Independent Scale CheckBox
	ImGui::Text("Independent Of Object Scale");
	ImGui::SameLine(0.f, 10.f);
	ImGui::Checkbox("##Collider2DIndependent", &m_Independent);

	ApplyColliderDetail();
}

void Collider2DUI::ApplyColliderDetail()
{
	m_ColliderCom->SetScale(Vec3(m_Scale.x, m_Scale.y, m_Scale.z));
	m_ColliderCom->SetOffset(Vec3(m_Offset.x, m_Offset.y, m_Offset.z));
	m_ColliderCom->SetIndependentScale(m_Independent);
}
