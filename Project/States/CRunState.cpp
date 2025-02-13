#include "pch.h"
#include "CRunState.h"

CRunState::CRunState()
{
}

CRunState::~CRunState()
{
}

void CRunState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Run.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Run.flip");

	m_AttackPower = 0;
	m_IsAttackState = false;
}

void CRunState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, true);

	m_Speed = GetBlackboardData<float>(L"Speed");
	m_Friction = GetBlackboardData<float>(L"Friction");

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CRunState::FinalTick()
{
	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_ReachNoPlatformCol = GetBlackboardData<INT>(L"ReachNoPlatformCollider");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");
	Vec3 vVelocity = m_Player->RigidBody()->GetVelocity();
	float velocityMagnitude = vVelocity.Length();

	if (KEY_TAP(KEY::A))
	{
		m_Player->FSM()->ChangeState(L"AttackLight0");
	}
	if (KEY_TAP(KEY::LSHIFT))
	{
		GetFSM()->ChangeState(L"Dash");
	}

	if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
	{
		switch (m_Dir)
		{
		case UNITVEC_TYPE::LEFT:
			GetFSM()->ChangeState(L"Idle");
			m_Player->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
			break;

		case UNITVEC_TYPE::RIGHT:
			
			GetFSM()->ChangeState(L"Idle");
			m_Player->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			break;
		}
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		if (m_ReachMapLimit != 1 && m_ReachNoPlatformCol != 1)
		{
			m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));

			if (vVelocity.x > 0.f)
				m_Player->RigidBody()->SetFriction(m_Friction + pow(velocityMagnitude, 2) * 100.f);
			else
				m_Player->RigidBody()->SetFriction(m_Friction);
		}
		else
		{
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		}
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_Dir = UNITVEC_TYPE::RIGHT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		if (m_ReachMapLimit != 2 && m_ReachNoPlatformCol != 2)
		{
			m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));

			if (vVelocity.x < 0.f)
				m_Player->RigidBody()->SetFriction(m_Friction + pow(velocityMagnitude, 2) * 100.f);
			else
				m_Player->RigidBody()->SetFriction(m_Friction);
		}
		else
		{
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		}
	}

	//switch (m_Dir)
	//{
	//case UNITVEC_TYPE::LEFT:
	//	if (m_ReachMapLimit != 1)
	//	{
	//		m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));
	//
	//		if (vVelocity.x > 0.f)
	//			m_Player->RigidBody()->SetFriction(m_Friction + pow(velocityMagnitude, 2) * 500.f);
	//		else
	//			m_Player->RigidBody()->SetFriction(m_Friction);
	//	}
	//	else
	//	{
	//		m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	//	}
	//		
	//	break;
	//
	//case UNITVEC_TYPE::RIGHT:
	//	if (m_ReachMapLimit != 2)
	//	{
	//		m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));
	//
	//		if (vVelocity.x < 0.f)
	//			m_Player->RigidBody()->SetFriction(m_Friction + pow(velocityMagnitude, 2) * 500.f);
	//		else
	//			m_Player->RigidBody()->SetFriction(m_Friction);
	//	}
	//	else
	//	{
	//		m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	//	}
	//		
	//	break;
	//}

	if (KEY_TAP(KEY::SPACE))
	{
		m_Owner->ChangeState(L"Jump");
	}
}

void CRunState::Exit()
{
	m_Player->RigidBody()->SetFriction(m_Friction);
}


