#include "pch.h"
#include "CReachLimitState.h"

CReachLimitState::CReachLimitState()
{
}

CReachLimitState::~CReachLimitState()
{
}

void CReachLimitState::Set()
{
	CCameraState::Set();

	m_springConstant = 1.2f;
	m_dampingFactor = 0.5f;
	m_Velocity = Vec3(0.f, 0.f, 0.f);
}

void CReachLimitState::Enter()
{
	m_IsReachLimit = true;
	m_LimitTargetPos = GetBlackboardData<Vec3>(L"LimitTargetPos");
	m_LimitTargetScale = GetBlackboardData<Vec3>(L"LimitTargetScale");
	
	m_Velocity = m_Camera->RigidBody()->GetVelocity();
	float VelocityMagnitude = m_Velocity.Length();

	Vec3 vUnit = m_Velocity.Normalize() * -1.f;

	float Speed = 0.f;
	if (VelocityMagnitude <= 10.f)
	{
		Speed = VelocityMagnitude;
	}
	else
	{
		Speed = VelocityMagnitude * 110.f;
	}
	

	Vec3 springForce = vUnit * Speed * m_springConstant;
	Vec3 dampingForce = -m_Velocity * m_dampingFactor;

	Vec3 totalForce = springForce + dampingForce;

	m_Camera->RigidBody()->AddForce(totalForce);
}

void CReachLimitState::FinalTick()
{
	Vec3 vPlayerPos = GetBlackboardData<Vec3>(L"PlayerPos");

	if (m_Velocity.Normalize().x > 0)
	{
		if (m_Camera->Transform()->GetRelativePos().x > vPlayerPos.x)
			m_IsReachLimit = false;
	}
	else if (m_Velocity.Normalize().x < 0)
	{
		if (m_Camera->Transform()->GetRelativePos().x < vPlayerPos.x)
			m_IsReachLimit = false;
	}
	
	if (!m_IsReachLimit)
	{
		m_Owner->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_IsReachLimit);
		m_Owner->ChangeState(L"ChasingPlayer");
	}
		


	//float StandardPosX = 0.f;
	//float distance = 0.f;
	//
	//if (m_Velocity.Normalize().x > 0)
	//{
	//	StandardPosX = m_LimitTargetPos.x - (m_LimitTargetScale.x * 0.5f);
	//	distance = StandardPosX - vPlayerPos.x;
	//}
	//else if (m_Velocity.Normalize().x < 0)
	//{
	//	StandardPosX = m_LimitTargetPos.x + (m_LimitTargetScale.x * 0.5f);
	//	distance = vPlayerPos.x - StandardPosX;
	//}
	//
	//if (fabs(distance) > 700.f)
	//{
	//	m_IsReachLimit = false;
	//	m_Owner->ChangeState(L"ChasingPlayer");
	//	m_Owner->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_IsReachLimit);
	//}
}

void CReachLimitState::Exit()
{
}


