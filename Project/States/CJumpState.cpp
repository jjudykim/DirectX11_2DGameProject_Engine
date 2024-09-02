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
}

void CJumpState::FinalTick()
{
}

void CJumpState::Exit()
{
}


