#include "pch.h"
#include "CBossChainState.h"

#include "Engine\CLevelMgr.h"

CBossChainState::CBossChainState()
{
}

CBossChainState::~CBossChainState()
{
}

void CBossChainState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Chain.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Chain.flip");

	m_BossChain = CLevelMgr::GetInst()->FindObjectByName(L"TriggerChain");
	m_BossChain->FlipBookComponent()->SetFinish(true);
}

void CBossChainState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(586.f, 640.f, 0.f));

	m_MonsterSpawnState = 1;
	m_Boss->FSM()->SetBlackboardData(L"MonsterSpawnState", DATA_TYPE::INT, &m_MonsterSpawnState);

	CTimeMgr::GetInst()->AddTimer(0.2f, [this]() { m_BossChain->FlipBookComponent()->Play(0, false); }, false);
}

void CBossChainState::FinalTick()
{
}

void CBossChainState::Exit()
{
	m_BossChain->FlipBookComponent()->SetCurFrame(0);
}

