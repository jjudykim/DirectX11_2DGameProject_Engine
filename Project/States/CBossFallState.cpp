#include "pch.h"
#include "CBossFallState.h"

CBossFallState::CBossFallState()
{
}

CBossFallState::~CBossFallState()
{
}

void CBossFallState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Fall.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Fall.flip");
}

void CBossFallState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativeScale(Vec3(320.f, 320.f, 0.f));
	m_Boss->Transform()->SetRelativePos(Vec3(573.f, 625.f, 0.f));

	m_Time = 0.f;
	m_ResetTime = false;
}

void CBossFallState::FinalTick()
{
	Vec3 vBossPos = m_Boss->Transform()->GetRelativePos();

	if (vBossPos.x > 0)
	{	
		float vSpeed = sinf(m_Time * PI);
		m_Boss->Transform()->SetRelativePos(Vec3(vBossPos.x - (950.f * vSpeed * DT), vBossPos.y, vBossPos.z));
		if (m_Time < 1)
			m_Time += DT;
	}
	else
	{
		if (m_Time > 0.f && m_ResetTime == false)
		{
			m_Time = 0.f;
			m_ResetTime = true;
		}

		float vSpeed = sinf(m_Time * PI);
		m_Boss->Transform()->SetRelativePos(Vec3(vBossPos.x, vBossPos.y - (1200.f * vSpeed * DT), vBossPos.z));
		m_Time += DT;

		if (vBossPos.y < 10.f)
			m_Boss->FSM()->ChangeState(L"BossGroggy");
	}
}

void CBossFallState::Exit()
{
}


