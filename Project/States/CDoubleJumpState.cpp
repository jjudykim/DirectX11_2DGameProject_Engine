#include "pch.h"
#include "CDoubleJumpState.h"

CDoubleJumpState::CDoubleJumpState()
{
}

CDoubleJumpState::~CDoubleJumpState()
{
}

void CDoubleJumpState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_DoubleJump.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_DoubleJump.flip");

	m_AttackPower = 0;
	m_IsAttackState = false;
}

void CDoubleJumpState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, false);

	m_Player->RigidBody()->SetJumpSpeed(GetBlackboardData<float>(L"JumpSpeed"));
	GetTargetObject()->RigidBody()->Jump();

	m_Speed = GetBlackboardData<float>(L"Speed");

	m_JumpCount += 1;
	m_Player->FSM()->SetBlackboardData(L"JumpCount", DATA_TYPE::INT, &m_JumpCount);
	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CDoubleJumpState::FinalTick()
{
	if (m_Player->RigidBody()->GetVelocityByGravity().y <= 0)
	{
		m_Owner->ChangeState(L"Fall");
	}

	if (KEY_TAP(KEY::A))
	{
		m_Player->FSM()->ChangeState(L"AttackLight0");
	}

	if (KEY_TAP(KEY::D))
	{
		m_Player->FSM()->ChangeState(L"AttackSmash");
	}

	if (KEY_TAP(KEY::LSHIFT))
	{
		GetFSM()->ChangeState(L"Dash");
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
}

void CDoubleJumpState::Exit()
{
}


