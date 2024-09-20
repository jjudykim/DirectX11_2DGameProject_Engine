#include "pch.h"
#include "CBossJumpBackState.h"

CBossJumpBackState::CBossJumpBackState()
{
}

CBossJumpBackState::~CBossJumpBackState()
{
}

void CBossJumpBackState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_JumpBack.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_JumpBack.flip");
}

void CBossJumpBackState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Boss->Transform()->SetRelativePos(Vec3(-20.f, 30.f, 0.f));
	m_Boss->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 0.f));
	m_Boss->Collider2D()->SetScale(Vec3(0.f, 0.f, 0.f));

	m_Time = 0;
	m_ResetTime = false;
	m_PassPeakPoint = false;
}

void CBossJumpBackState::FinalTick()
{
	Vec3 vBossPos = m_Boss->Transform()->GetRelativePos();

	if (vBossPos.x < 150.f)
	{
		float vSpeed = sinf(m_Time * PI);

		m_Boss->Transform()->SetRelativePos(Vec3(vBossPos.x + (200.f * DT), vBossPos.y + (120.f * vSpeed * DT), vBossPos.z));
		m_Time += (DT * 3.f);

	}
	else
	{
		if (m_Time > 0.f && m_ResetTime == false)
		{
			m_Time = 0.f;
			m_ResetTime = true;
		}

		float fSpeed = sinf(m_Time * PI);
		float fDoubleSpeed = fabs(pow(sinf(m_Time * PI), 2));

		if (m_Time < 1.0f)
		{
			m_Boss->Transform()->SetRelativePos(Vec3(vBossPos.x + (800.f * fDoubleSpeed * DT * 1.2f), vBossPos.y + (1150.f * fSpeed * DT * 1.2f), vBossPos.z));
		}
		else
		{
			m_Boss->Transform()->SetRelativePos(Vec3(vBossPos.x, vBossPos.y + (1100.f * fSpeed * DT * 1.2f), vBossPos.z));
		}
		

		if (m_Time < 1.3f)
			m_Time += DT * 1.2f;
		else
			m_Boss->FSM()->ChangeState(L"BossIdle");
	}
}

void CBossJumpBackState::Exit()
{
}