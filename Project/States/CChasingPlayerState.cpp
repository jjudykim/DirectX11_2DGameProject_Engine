#include "pch.h"
#include "CChasingPlayerState.h"

#include "Engine/CLevelMgr.h"

CChasingPlayerState::CChasingPlayerState()
{
}

CChasingPlayerState::~CChasingPlayerState()
{
}

void CChasingPlayerState::Set()
{
}

void CChasingPlayerState::Enter()
{
	
}

void CChasingPlayerState::FinalTick()
{
	if (KEY_PRESSED(KEY::_1))
	{
		m_Owner->ChangeState(L"DirectControl");
	}

	m_CamSpeed = GetBlackboardData<float>(L"CamSpeed");
	m_StandardPos = GetBlackboardData<Vec3>(L"StandardPos");

	Vec3 vPos = GetTargetObject()->Transform()->GetRelativePos();

	if (fabs(vPos.x - m_StandardPos.x) > 100 || fabs(vPos.y - m_StandardPos.y) > 50)
	{
		Vec3 vUnit = m_StandardPos - vPos;
		vUnit = vUnit.Normalize();
		vPos += vUnit * DT * m_CamSpeed;
	}

	GetTargetObject()->Transform()->SetRelativePos(vPos);
}

void CChasingPlayerState::Exit()
{
}


