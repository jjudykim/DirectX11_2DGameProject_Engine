#include "pch.h"
#include "CBossGroggyState.h"

CBossGroggyState::CBossGroggyState()
{
}

CBossGroggyState::~CBossGroggyState()
{
}

void CBossGroggyState::Set()
{
	CBossState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Boss_Groggy.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Boss_Groggy.flip");
}

void CBossGroggyState::Enter()
{
	m_FBCom->Play(m_FBIdx, true);

	m_Boss->Transform()->SetRelativePos(Vec3(-20.f, 10.f, 0.f));
	m_Boss->Transform()->SetRelativeScale(Vec3(250.f, 250.f, 0.f));
	m_Boss->Collider2D()->SetIndependentScale(true);
	m_Boss->Collider2D()->SetScale(Vec3(0.50f, 0.50f, 0.f));
	m_Boss->Collider2D()->SetOffset(Vec3(0.f, -0.001f, 0.f));
}

void CBossGroggyState::FinalTick()
{
	if (KEY_TAP(KEY::ALT))
	{
		m_Boss->FSM()->ChangeState(L"BossJumpBack");
	}
}

void CBossGroggyState::Exit()
{
}


