#include "pch.h"
#include "CIdleState.h"

CIdleState::CIdleState()
{
}

CIdleState::~CIdleState()
{
}

void CIdleState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Idle.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Idle.flip");
}

void CIdleState::Enter()
{
	m_FBCom->Play(m_FBIdx, m_FBCom->GetRepeat());
}

void CIdleState::FinalTick()
{
	m_Dir = GetBlackboardData<UNITVEC_TYPE>(L"Dir");

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		GetTargetObject()->FSM()->ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_Dir = UNITVEC_TYPE::RIGHT;
		GetTargetObject()->FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		GetTargetObject()->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		GetTargetObject()->FSM()->ChangeState(L"Run");
	}

	UNITVEC_TYPE check = GetBlackboardData<UNITVEC_TYPE>(L"Dir");
}

void CIdleState::Exit()
{
}