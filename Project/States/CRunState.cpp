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

	switch (m_Dir)
	{
	case UNITVEC_TYPE::LEFT:
		if (m_ReachMapLimit != 1)
			m_Player->RigidBody()->AddForce(Vec3((-1.f) * m_Speed, 0.f, 0.f));
			//vPos.x -= DT * m_Speed;
		break;

	case UNITVEC_TYPE::RIGHT:
		if (m_ReachMapLimit != 2)
			m_Player->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));
			//vPos.x += DT * m_Speed;
		break;
	}

	//if (KEY_TAP(KEY::SPACE))
	//{
	//	m_Owner->ChangeState(L"Jump");
	//}
}

void CRunState::Exit()
{
}


