#include "pch.h"
#include "CDamageState.h"

CDamageState::CDamageState()
{
}

CDamageState::~CDamageState()
{
	
}

void CDamageState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Damage.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Damage.flip");
}

void CDamageState::Enter()
{
	if (m_CurFB != m_FBCom->GetCurFlipBook())
		m_FBCom->Play(m_FBIdx, false);
	
	m_GodMode = true;
	m_Player->FSM()->SetBlackboardData(L"PlayerGodMode", DATA_TYPE::INT, &m_GodMode);
}

void CDamageState::FinalTick()
{
	if (m_FBCom->GetFinish())
	{
		m_Player->FSM()->ChangeState(L"Idle");
	}
}

void CDamageState::Exit()
{
	//m_Player->MeshRender()->GetMaterial()->SetUseBlinkEffect(false);
}