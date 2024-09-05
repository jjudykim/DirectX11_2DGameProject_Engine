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

	GetTargetObject()->RigidBody()->Jump();

	m_JumpCount += 1;
}

void CJumpState::FinalTick()
{
	if (KEY_TAP(KEY::SPACE))
	{
		m_Owner->ChangeState(L"DoubleJump");
	}

	if (m_Player->RigidBody()->GetVelocityByGravity().y <= 0)
	{
		m_Owner->ChangeState(L"Fall");
	}

	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");
	Vec3 vVelocity = m_Player->RigidBody()->GetVelocity();
	float velocityMagnitude = vVelocity.Length();

	switch (m_Dir)
	{
	case UNITVEC_TYPE::LEFT:
		if (m_ReachMapLimit != 1)
			m_Player->RigidBody()->AddForce(Vec3((-1.f) * velocityMagnitude, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

		break;

	case UNITVEC_TYPE::RIGHT:
		if (m_ReachMapLimit != 2)
			m_Player->RigidBody()->AddForce(Vec3(velocityMagnitude, 0.f, 0.f));
		else
			m_Player->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		break;
	}
}

void CJumpState::Exit()
{
}


