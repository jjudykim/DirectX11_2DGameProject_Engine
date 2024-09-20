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
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(573.f, 645.f, 0.f));
}

void CBossAttackState::FinalTick()
{
	m_CurPhase = GetBlackboardData<int>(L"CurPhase");

	if (m_CurPhase == 1)
	{

	}

	if (m_FBCom->GetFinish())
	{
		m_Boss->FSM()->ChangeState(L"BossIdle");
	}
}

void CBossAttackState::Exit()
{
}


