#include "pch.h"
#include "CRunState.h"

CRunState::CRunState()
{
}

CRunState::~CRunState()
{
}

void CRunState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Run.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Run.flip");
}

void CRunState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, m_FBCom->GetRepeat());

	m_Speed = GetBlackboardData<float>(L"Speed");
}

void CRunState::FinalTick()
{
	m_ReachMapLimit = GetBlackboardData<INT>(L"ReachMapLimit");
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");

	if (KEY_RELEASED(KEY::LEFT) || KEY_RELEASED(KEY::RIGHT))
	{
		switch (m_Dir)
		{
		case UNITVEC_TYPE::LEFT:
			GetFSM()->ChangeState(L"Idle");
			m_Player->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
			break;

		case UNITVEC_TYPE::RIGHT:
			
			GetFSM()->ChangeState(L"Idle");
			m_Player->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
			break;
		}
	}
	
	Vec3 vPos = m_Player->Transform()->GetRelativePos();

	switch (m_Dir)
	{
	case UNITVEC_TYPE::LEFT:
		if (m_ReachMapLimit != 1)
			vPos.x -= DT * m_Speed;
		break;

	case UNITVEC_TYPE::RIGHT:
		if (m_ReachMapLimit != 2)
			vPos.x += DT * m_Speed;
		break;
	}
	
	m_Player->Transform()->SetRelativePos(vPos);
}

void CRunState::Exit()
{
}


