#include "pch.h"
#include "CDashState.h"

CDashState::CDashState()
{
}

CDashState::~CDashState()
{
}

void CDashState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Dash.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Dash.flip");

	m_AttackPower = 0;
	m_IsAttackState = false;
}

void CDashState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, false);

	m_ReachNoPlatformCol = GetBlackboardData<INT>(L"ReachNoPlatformCollider");
	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");
	m_MaxWalkSpeed = GetBlackboardData<float>(L"MaxWalkSpeed");

	Vec3 vVelocity = m_Player->RigidBody()->GetVelocity();
	m_Player->RigidBody()->SetMaxWalkSpeed(m_MaxWalkSpeed * 1.5f);

	switch (m_Dir)
	{
	case UNITVEC_TYPE::LEFT:
		if (m_ReachMapLimit != 1 && m_ReachNoPlatformCol != 1)
			m_Player->RigidBody()->SetVelocity(Vec3(-5000.f, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

		break;

	case UNITVEC_TYPE::RIGHT:
		if (m_ReachMapLimit != 2 && m_ReachNoPlatformCol != 1)
			m_Player->RigidBody()->SetVelocity(Vec3(5000.f, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

		break;
	}

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CDashState::FinalTick()
{
	if (m_FBCom->GetFinish())
	{
		CState* PrevState = GetFSM()->GetPrevState();
		if (PrevState == GetFSM()->FindState(L"Jump")
			|| PrevState == GetFSM()->FindState(L"DoubleJump")
			|| PrevState == GetFSM()->FindState(L"Fall"))
		{
			GetFSM()->ChangeState(L"Fall");
		}
		else
		{
			GetFSM()->ChangeState(L"Idle");
		}
	}

	Vec3 vVelocity = m_Player->RigidBody()->GetVelocity();
	float velocityMagnitude = vVelocity.Length();

	switch (m_Dir)
	{
	case UNITVEC_TYPE::LEFT:
		if (m_ReachMapLimit != 1 && m_ReachNoPlatformCol != 1)
			m_Player->RigidBody()->AddForce(Vec3((-1.f) * velocityMagnitude, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

		break;

	case UNITVEC_TYPE::RIGHT:
		if (m_ReachMapLimit != 2 && m_ReachNoPlatformCol != 2)
			m_Player->RigidBody()->AddForce(Vec3(velocityMagnitude, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

		break;
	}

}

void CDashState::Exit()
{
	m_Player->RigidBody()->SetMaxWalkSpeed(m_MaxWalkSpeed);
}


