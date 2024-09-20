#include "pch.h"
#include "CBossIdleState.h"

CBossIdleState::CBossIdleState()
{
}

CBossIdleState::~CBossIdleState()
{
}

void CBossIdleState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Idle.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Idle.flip");
}

void CBossIdleState::Enter()
{
	m_FBCom->Play(m_FBIdx, true);

	m_Boss->Transform()->SetRelativeScale(Vec3(225.f, 225.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(573.f, 625.f, 0.f));

	//CTimeMgr::GetInst()->AddTimer(2.0f, [this]() { m_Boss->FSM()->ChangeState(L"BossAttack"); }, false);
}

void CBossIdleState::FinalTick()
{
	m_CurPhase = GetBlackboardData<int>(L"CurPhase");
}

void CBossIdleState::Exit()
{
}


