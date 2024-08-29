#include "pch.h"
#include "CPlayerState.h"


CPlayerState::CPlayerState()
	: m_Player(nullptr)
{ 
}

CPlayerState::~CPlayerState()
{
}


void CPlayerState::Set()
{
	if (m_Player == nullptr)
		m_Player = GetTargetObject();

	if (m_FBCom == nullptr)
		m_FBCom = m_Player->FlipBookComponent();
}

void CPlayerState::Enter()
{
}

void CPlayerState::FinalTick()
{
}

void CPlayerState::Exit()
{
}
