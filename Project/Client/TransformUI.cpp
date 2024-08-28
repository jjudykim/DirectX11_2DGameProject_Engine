#include "pch.h"
#include "TransformUI.h"

TransformUI::TransformUI()
	: ComponentUI(COMPONENT_TYPE::TRANSFORM)
{
}

TransformUI::~TransformUI()
{
}

void TransformUI::Update()
{
	if (GetTargetObject() == nullptr || GetTargetObject()->IsDead())
		return;

	Title();

	CTransform* pTrans = GetTargetObject()->Transform();

	Vec3 vPos = pTrans->GetRelativePos();
	Vec3 vScale = pTrans->GetRelativeScale();
	Vec3 vRot = pTrans->GetRelativeRotation();
	vRot = (vRot / XM_PI) * 180.f;

	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Pos", vPos);

	ImGui::Text("Scale");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Scale", vScale);

	ImGui::Text("Rotation");
	ImGui::SameLine(100);
	ImGui::DragFloat3("##Rot", vRot, 0.1f);

	pTrans->SetRelativePos(vPos);
	pTrans->SetRelativeScale(vScale);
	vRot = (vRot / 180.f) * XM_PI;
	pTrans->SetRelativeRotation(vRot);


	// Independent Scale
	bool IS = pTrans->IsIndependentParent();

	ImGui::Text("Ignore Parent");
	ImGui::SameLine(100);
	if (ImGui::Checkbox("##TransIS", &IS))
	{
		pTrans->SetIndependentParent(IS);
	}
}
