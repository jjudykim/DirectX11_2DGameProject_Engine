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
}

void CIdleState::Enter()
{
	m_FBCom->Play(m_FBIdx, m_FBCom->GetRepeat());
}

void CIdleState::FinalTick()
{
}

void CIdleState::Exit()
{
}