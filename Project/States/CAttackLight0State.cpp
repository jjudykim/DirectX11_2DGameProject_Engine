#include "pch.h"
#include "CAttackLight0State.h"

CAttackLight0State::CAttackLight0State()
{
}

CAttackLight0State::~CAttackLight0State()
{
}

void CAttackLight0State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light0.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light0.flip");
}

void CAttackLight0State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);
}

void CAttackLight0State::FinalTick()
{
	if (KEY_TAP(KEY::A))
	{
		m_Player->FSM()->ChangeState(L"AttackLight1");
	}

	if (m_FBCom->GetFinish())
	{
		m_Player->FSM()->ChangeState(L"Idle");
	}

}

void CAttackLight0State::Exit()
{
}


