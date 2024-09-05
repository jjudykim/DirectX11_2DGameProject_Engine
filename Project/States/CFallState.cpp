#include "pch.h"
#include "CFallState.h"

CFallState::CFallState()
{
}

CFallState::~CFallState()
{
}

void CFallState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Fall.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Fall.flip");
}

void CFallState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, true);

	m_Speed = GetBlackboardData<float>(L"Speed");
}

void CFallState::FinalTick()
{
	if (KEY_TAP(KEY::SPACE))
	{
		if (m_JumpCount < 2)
			m_Owner->ChangeState(L"DoubleJump");
	}

	if (KEY_TAP(KEY::LSHIFT))
	{
		GetFSM()->ChangeState(L"Dash");
	}

	if (m_Player->RigidBody()->IsGround())
		m_Owner->ChangeState(L"Idle");

	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		if (m_ReachMapLimit != 1)
		{
			m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));

			if (m_ReachMapLimit != 1)
				m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));
			else
				m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
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
		if (m_ReachMapLimit != 2)
		{
			m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));

			if (m_ReachMapLimit != 2)
				m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));
			else
				m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
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
	//		m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));
	//	else
	//		m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	//
	//	break;
	//
	//case UNITVEC_TYPE::RIGHT:
	//	if (m_ReachMapLimit != 2)
	//		m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));
	//	else
	//		m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	//	break;
	//}
}

void CFallState::Exit()
{
}


