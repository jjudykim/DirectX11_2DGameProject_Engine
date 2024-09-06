#include "pch.h"
#include "RigidBodyUI.h"

#include <Engine/CLevelMgr.h>

RigidBodyUI::RigidBodyUI()
	: ComponentUI(COMPONENT_TYPE::RIGIDBODY)
{
}

RigidBodyUI::~RigidBodyUI()
{
}

void RigidBodyUI::Init()
{
	m_Mass = 0;
	m_InitWalkSpeed = 0;
	m_MaxWalkSpeed = 0;
	m_Friction = 0;
	m_GravityAccel = 0;
	m_MaxGravitySpeed = 0;
	m_JumpSpeed = 0;
	m_UseGravity = false;
}

void RigidBodyUI::Update()
{
	if (GetTargetObject() == nullptr || GetTargetObject()->IsDead())
		return;

	Title();

	if (m_RigidBody == nullptr)
		Init();

	SetTargetObject(GetTargetObject());
	m_RigidBody = GetTargetObject()->RigidBody();

	GetRigidBodyData(m_RigidBody);

	ImGui::Text("Velocity");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputFloat3("##DragVelocity", m_Velocity, "%.2f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("VelocityByGrav");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputFloat3("##DragVelocity", m_VelocityByGravity, "%.2f", ImGuiInputTextFlags_ReadOnly);

	ImGui::Separator();
	ImGui::Text("Mass");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##DragMass", &m_Mass, 0.1f, 0.0f, 0.0f, "%.2f");

	ImGui::Text("Friction");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##DragFriction", &m_Friction, 1.f, 0.f, 0.f, "%.2f");
	
	ImGui::Separator();
	ImGui::Text("Min WalkSpeed");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##DragMinWalkSpeed", &m_InitWalkSpeed, 1.f, 0.f, 0.f, "%.2f");

	ImGui::Text("Max WalkSpeed");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##DragMaxWalkSpeed", &m_MaxWalkSpeed, 1.f, 0.f, 0.f, "%.2f");

	ImGui::Separator();
	ImGui::Text("Use Gravity");
	ImGui::SameLine(120.f);
	ImGui::Checkbox("##UseGravity", &m_UseGravity);

	ImGui::Text("Jump Speed");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##JumpSpeed", &m_JumpSpeed, 1.f, 0.f, 0.f, "%.2f");

	ImGui::Text("Gravity Accel");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##GravityAccel", &m_GravityAccel, 1.f, 0.f, 0.f, "%.2f");

	ImGui::Text("MaxGravitySpeed");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::DragFloat("##MaxGravitySpeed", &m_MaxGravitySpeed, 1.f, 0.f, 0.f, "%.2f");

	SetRigidBodyData(m_RigidBody);
}

void RigidBodyUI::GetRigidBodyData(CRigidBody* _RigidBody)
{
	Vec3 vVelocity           = _RigidBody->GetVelocity();
	m_Velocity[0]            = vVelocity.x;
	m_Velocity[1]            = vVelocity.y;
	m_Velocity[2]            = vVelocity.z;

	Vec3 vVelocityByGravity  = _RigidBody->GetVelocityByGravity();
	m_VelocityByGravity[0]   = vVelocityByGravity.x;
	m_VelocityByGravity[1]   = vVelocityByGravity.y;
	m_VelocityByGravity[2]   = vVelocityByGravity.z;

	m_Mass                   = _RigidBody->GetMass();
	m_InitWalkSpeed          = _RigidBody->GetInitialWalkSpeed();
	m_MaxWalkSpeed           = _RigidBody->GetMaxWalkSpeed();
	m_Friction               = _RigidBody->GetFriction();
	m_GravityAccel           = _RigidBody->GetGravityAccel();
	m_MaxGravitySpeed        = _RigidBody->GetMaxGravitySpeed();
	m_JumpSpeed              = _RigidBody->GetJumpSpeed();
	m_UseGravity             = _RigidBody->IsUseGravity();
}

void RigidBodyUI::SetRigidBodyData(CRigidBody* _RigidBody)
{
	_RigidBody->SetMass(m_Mass);
	_RigidBody->SetInitialWalkSpeed(m_InitWalkSpeed);
	_RigidBody->SetMaxWalkSpeed(m_MaxWalkSpeed);
	_RigidBody->SetFriction(m_Friction);
	_RigidBody->SetGravityAccel(m_GravityAccel);
	_RigidBody->SetMaxGravitySpeed(m_MaxGravitySpeed);
	_RigidBody->SetJumpSpeed(m_JumpSpeed);
	_RigidBody->SetUseGravity(m_UseGravity);
}