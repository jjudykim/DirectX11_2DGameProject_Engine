#include "pch.h"
#include "CIdleState.h"

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Idle.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Idle.flip");

	m_AttackPower = 0;
	m_IsAttackState = false;
}

void CIdleState::Enter()
{
	m_FBCom->Play(m_FBIdx, true);

	m_JumpCount = 0;

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CIdleState::FinalTick()
{
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");

	if (KEY_TAP(KEY::A))
	{
		m_Player->FSM()->ChangeState(L"AttackLight0");
	}

	if (KEY_TAP(KEY::D))
	{
		m_Player->FSM()->ChangeState(L"AttackHeavy0");
	}

	if (KEY_TAP(KEY::T))
	{
		m_Player->FSM()->ChangeState(L"Damage");
	}

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		m_Player->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		m_Player->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		m_Player->FSM()->ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_Dir = UNITVEC_TYPE::RIGHT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		GetTargetObject()->FSM()->ChangeState(L"Run");
	}

	if (KEY_TAP(KEY::SPACE))
	{
		m_Owner->ChangeState(L"Jump");
	}

	if (!m_Player->RigidBody()->IsGround())
	{
		m_Owner->ChangeState(L"Fall");
	}

	UNITVEC_TYPE check = GetBlackboardData<UNITVEC_TYPE>(L"Dir");
}

void CIdleState::Exit()
{
}