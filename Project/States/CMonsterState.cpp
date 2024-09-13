#include "pch.h"
#include "CMonsterState.h"

CMonsterState::CMonsterState()
{
}

CMonsterState::~CMonsterState()
{
}

void CMonsterState::Set()
{
	if (m_Monster == nullptr)
		m_Monster = GetTargetObject();

	if (m_FBCom == nullptr)
		m_FBCom = m_Monster->FlipBookComponent();

	m_MonsterType = GetBlackboardData<int>(L"MonsterType");
	m_Monster->FSM()->SetBlackboardData(L"TargetDir", DATA_TYPE::UNITVEC_TYPE, &m_TargetDir);
}

void CMonsterState::Enter()
{
}

void CMonsterState::FinalTick()
{
}

void CMonsterState::Exit()
{
}


