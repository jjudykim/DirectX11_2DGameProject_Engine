#include "pch.h"
#include "CBossDeadState.h"

CBossDeadState::CBossDeadState()
{
}

CBossDeadState::~CBossDeadState()
{
}

void CBossDeadState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Dead.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Dead.flip");
}

void CBossDeadState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);
}

void CBossDeadState::FinalTick()
{
}

void CBossDeadState::Exit()
{
}


