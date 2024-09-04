#include "pch.h"
#include "CCameraState.h"

CCameraState::CCameraState()
{
}

CCameraState::~CCameraState()
{
}

void CCameraState::Set()
{
	if (m_Camera == nullptr)
		m_Camera = GetTargetObject();
}

void CCameraState::Enter()
{
}

void CCameraState::FinalTick()
{
}

void CCameraState::Exit()
{
}


