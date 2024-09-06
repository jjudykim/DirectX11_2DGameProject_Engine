#include "pch.h"
#include "CJumpState.h"

CJumpState::CJumpState()
{
}

CJumpState::~CJumpState()
{
}

void CJumpState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Jump.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Jump.flip");
}

void CJumpState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, false);

	m_Player->RigidBody()->SetJumpSpeed(GetBlackboardData<float>(L"JumpSpeed"));
	m_Player->RigidBody()->Jump();

	m_Speed = GetBlackboardData<float>(L"Speed");

	m_JumpCount += 1;
}

void CJumpState::FinalTick()
{
	if (KEY_TAP(KEY::SPACE))
	{
		m_Owner->ChangeState(L"DoubleJump");
	}

	if (KEY_TAP(KEY::LSHIFT))
	{
		GetFSM()->ChangeState(L"Dash");
	}

	if (m_Player->RigidBody()->GetVelocityByGravity().y <= 0)
	{
		m_Owner->ChangeState(L"Fall");
	}

	m_ReachNoPlatformCol = GetBlackboardData<INT>(L"ReachNoPlatformCollider");
	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		if (m_ReachMapLimit != 1 && m_ReachNoPlatformCol != 1)
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
		if (m_ReachMapLimit != 2 && m_ReachNoPlatformCol != 2)
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

void CJumpState::Exit()
{
}


