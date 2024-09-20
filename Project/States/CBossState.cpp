#include "pch.h"
#include "CBossState.h"

CBossState::CBossState()
	: m_Boss(nullptr)
{
}

CBossState::~CBossState()
{
}

void CBossState::Set()
{
	if (m_Boss == nullptr)
		m_Boss = GetTargetObject();

	if (m_FBCom == nullptr)
		m_FBCom = m_Boss->FlipBookComponent();
}

void CBossState::Enter()
{
}

void CBossState::FinalTick()
{
}

void CBossState::Exit()
{
}


