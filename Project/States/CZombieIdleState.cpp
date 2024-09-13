#include "pch.h"
#include "CZombieIdleState.h"

CZombieIdleState::CZombieIdleState()
{
}

CZombieIdleState::~CZombieIdleState()
{
}

void CZombieIdleState::Set()
{
	CMonsterState::Set();

	if (m_MonsterType == 1)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie0_Idle.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie0_Idle.flip");
	}
	else if (m_MonsterType == 2)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie1_Idle.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie1_Idle.flip");
	}

	m_IsAttacking = false;
}

void CZombieIdleState::Enter()
{
	m_FBCom->Play(m_FBIdx, true);

	m_Monster->Collider2D()->SetScale(Vec3(3.f, 0.7f, 0.0f));
	m_Monster->FSM()->SetBlackboardData(L"IsMonsterAttackState", DATA_TYPE::INT, &m_IsAttacking);
}

void CZombieIdleState::FinalTick()
{
	if (GetBlackboardData<int>(L"OverlapWithPlayer") == 1)
	{
		m_Monster->FSM()->ChangeState(L"ZombieChase");
		m_OverlapWithPlayer = 0;
		m_Monster->FSM()->SetBlackboardData(L"OverlapWithPlayer", DATA_TYPE::INT, &m_OverlapWithPlayer);
	}
}

void CZombieIdleState::Exit()
{
}


