#include "pch.h"
#include "CBossAttackState.h"

CBossAttackState::CBossAttackState()
{
}

CBossAttackState::~CBossAttackState()
{
}

void CBossAttackState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Attack.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Attack.flip");
}

void CBossAttackState::Enter()
{
	m_CurPhase = GetBlackboardData<int>(L"CurPhase");
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(573.f, 645.f, 0.f));

	if (m_CurPhase == 1)
	{
		m_ShootProjectile = 1;
		m_Boss->FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);
	}
	else if (m_CurPhase == 2)
	{
		m_ShootProjectile = 2;
		m_Boss->FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);
	}
	else if (m_CurPhase == 3)
	{
		m_ShootProjectile = rand() % 2 + 1;
		m_Boss->FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);
	}
}

void CBossAttackState::FinalTick()
{
	m_CurPhase = GetBlackboardData<int>(L"CurPhase");

	if (m_FBCom->GetFinish())
	{
		m_Boss->FSM()->ChangeState(L"BossIdle");
	}
}

void CBossAttackState::Exit()
{
}


