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
	m_PhaseStep = GetBlackboardData<int>(L"PhaseStep");
	m_MonsterSpawnState = GetBlackboardData<int>(L"MonsterSpawnState");

	if (m_PhaseStep == 0 || m_PhaseStep == 1)
	{
		if (m_CurPhase == 1)
		{
			if (m_Time > 1.5f)
			{
				m_Boss->FSM()->ChangeState(L"BossAttack");
				m_Time = 0.f;
			}
		}
		else if (m_CurPhase == 2)
		{
			if (m_Time > 2.5f)
			{
				m_Boss->FSM()->ChangeState(L"BossAttack");
				m_Time = 0.f;
			}
		}
		else if (m_CurPhase == 3)
		{
			if (m_Time > 1.5f)
			{
				m_Boss->FSM()->ChangeState(L"BossAttack");
				m_Time = 0.f;
			}
		}

		m_Time += DT;
	}

	if (m_PhaseStep == 2)
		m_Boss->FSM()->ChangeState(L"BossFall");

	if (m_PhaseStep == 3 && m_MonsterSpawnState < 1)
		m_Boss->FSM()->ChangeState(L"BossChain");
}

void CBossIdleState::Exit()
{
}


