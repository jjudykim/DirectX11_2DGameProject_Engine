#include "pch.h"
#include "CAttackLight1State.h"

CAttackLight1State::CAttackLight1State()
{
}

CAttackLight1State::~CAttackLight1State()
{
}

void CAttackLight1State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light1.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light1.flip");
}

void CAttackLight1State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);
}

void CAttackLight1State::FinalTick()
{
	if (KEY_TAP(KEY::A))
	{
		m_Player->FSM()->ChangeState(L"AttackLight2");
	}

	if (m_FBCom->GetFinish())
	{
		m_Player->FSM()->ChangeState(L"Idle");
	}
}

void CAttackLight1State::Exit()
{
}


