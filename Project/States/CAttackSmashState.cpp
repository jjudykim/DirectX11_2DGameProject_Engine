#include "pch.h"
#include "CAttackSmashState.h"

CAttackSmashState::CAttackSmashState()
{
}

CAttackSmashState::~CAttackSmashState()
{
}

void CAttackSmashState::Set()
{
	CPlayerState::Set();

	m_SmashFB[0] = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Smash0.flip");
	m_SmashFB[1] = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Smash1.flip");
	m_SmashFB[2] = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Smash2.flip");
	
	m_SmashFBIdx[0] = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Smash0.flip");
	m_SmashFBIdx[1] = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Smash1.flip");
	m_SmashFBIdx[2] = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Smash2.flip");
	
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Smash.flip");
}

void CAttackSmashState::Enter()
{
	m_CurFB = m_SmashFB[0];
	m_FBIdx = m_SmashFBIdx[0];
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(300.f, 300.f, 0.f));
	m_Weapon->Collider2D()->SetIndependentScale(true);
	m_Weapon->Collider2D()->SetScale(Vec3(0.5f, 0.5f, 0.f));

	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_Player->RigidBody()->SetVelocityByGravity(Vec3(0.f, -500.f, 0.f));
}

void CAttackSmashState::FinalTick()
{
	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();
	m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x, WeaponPos.y + 65, WeaponPos.z));

	if (m_FBCom->GetFinish())
	{
		if (m_FBIdx == m_SmashFBIdx[0])                                       // Pre -> Fall
		{
			m_CurFB = m_SmashFB[1];
			m_FBIdx = m_SmashFBIdx[1];
			m_FBCom->Play(m_FBIdx, true);
		}
		else if (m_FBIdx == m_SmashFBIdx[2])
		{
			m_Player->FSM()->ChangeState(L"Idle");
		}
	}

	if (m_FBIdx == m_SmashFBIdx[1] && m_Player->RigidBody()->IsGround())       // Fall -> Smash
	{
		m_CurFB = m_SmashFB[2];
		m_FBIdx = m_SmashFBIdx[2];
		m_FBCom->Play(m_FBIdx, false);
	}
}

void CAttackSmashState::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}