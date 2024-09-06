#include "pch.h"
#include "CAttackLight2State.h"

CAttackLight2State::CAttackLight2State()
{
}

CAttackLight2State::~CAttackLight2State()
{
}

void CAttackLight2State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light2.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light2.flip");
}

void CAttackLight2State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);
}

void CAttackLight2State::FinalTick()
{
	if (m_FBCom->GetFinish())
	{
		m_Player->FSM()->ChangeState(L"Idle");
	}
}

void CAttackLight2State::Exit()
{
}


