#include "pch.h"
#include "CZombieDamageState.h"

CZombieDamageState::CZombieDamageState()
{
}

CZombieDamageState::~CZombieDamageState()
{
}

void CZombieDamageState::Set()
{
	CMonsterState::Set();

	if (m_MonsterType == 1)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie0_Damage.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie0_Damage.flip");
		m_RelativeScale = GetBlackboardData<Vec3>(L"OriginScale") * 0.65f;
	}
	else if (m_MonsterType == 2)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie1_Damage.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie1_Damage.flip");
		m_RelativeScale = GetBlackboardData<Vec3>(L"OriginScale") * 0.8f;
	}

	m_IsAttacking = false;
}

void CZombieDamageState::Enter()
{
	m_Monster->Transform()->SetRelativeScale(m_RelativeScale);

	int bAlive = GetBlackboardData<int>(L"MonsterAlive");
	if (bAlive != 0)
	{
		CTimeMgr::GetInst()->AddTimer(2.0f, [this]() {m_Monster->FSM()->ChangeState(L"ZombieChase"); }, false);
	}
	m_FBCom->Play(m_FBIdx, true);

	m_Monster->RigidBody()->UseGravity(false);
	m_Monster->Collider2D()->SetScale(Vec3(0.f, 0.0f, 0.0f));

	m_Monster->FSM()->SetBlackboardData(L"IsMonsterAttackState", DATA_TYPE::INT, &m_IsAttacking);
}

void CZombieDamageState::FinalTick()
{
}

void CZombieDamageState::Exit()
{
	m_Monster->Transform()->SetRelativeScale(GetBlackboardData<Vec3>(L"OriginScale"));
	m_Monster->RigidBody()->UseGravity(false);
}


