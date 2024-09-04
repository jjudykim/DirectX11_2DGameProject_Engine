#include "pch.h"
#include "CChasingPlayerState.h"

#include "Engine/CLevelMgr.h"

CChasingPlayerState::CChasingPlayerState()
{
}

CChasingPlayerState::~CChasingPlayerState()
{
}

void CChasingPlayerState::Set()
{
	CCameraState::Set();	
	m_Friction = m_Camera->RigidBody()->GetFriction();
	m_MaxWalkSpeed = m_Camera->RigidBody()->GetMaxWalkSpeed();
}

void CChasingPlayerState::Enter()
{
}

void CChasingPlayerState::FinalTick()
{
	if (KEY_PRESSED(KEY::_1))
	{
		m_Owner->ChangeState(L"DirectControl");
	}

	m_CamSpeed = GetBlackboardData<float>(L"CamSpeed");
	m_StandardPos = GetBlackboardData<Vec3>(L"StandardPos");

	

	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();

	Vec3 vUnit = m_StandardPos - vPos;
	float distance = vUnit.Length();
	vUnit = vUnit.Normalize();

	//if (fabs(vPos.x - m_StandardPos.x) > 500 || fabs(vPos.x - m_StandardPos.y) > 100)
	//{
	//	//m_Camera->RigidBody()->SetFriction(m_Friction * 2.f);
	//	//m_Camera->RigidBody()->SetMaxWalkSpeed(m_MaxWalkSpeed * 2.f);
	//	m_Camera->RigidBody()->AddForce(vUnit * m_CamSpeed * 2.f);
	//}
	//
	//if (fabs(vPos.x - m_StandardPos.x) > 150 || fabs(vPos.y - m_StandardPos.y) > 50)
	//{
	//	//m_Camera->RigidBody()->SetFriction(m_Friction);
	//	//m_Camera->RigidBody()->SetMaxWalkSpeed(m_MaxWalkSpeed);
	//	m_Camera->RigidBody()->AddForce(vUnit * m_CamSpeed);
	//}

	float forceMagnitude = distance * m_CamSpeed;

	m_Camera->RigidBody()->AddForce(vUnit * forceMagnitude);

	DrawDebugRect(m_StandardPos, Vec3(50.f, 50.f, 0.f), Vec3(0.f, 0.f, 0.f), Vec4(1.0f, 1.0f, 0.3f, 1.f), 0.f, false);
}

void CChasingPlayerState::Exit()
{
}


