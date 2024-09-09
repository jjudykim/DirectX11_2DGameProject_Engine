#include "pch.h"
#include "CAttackLight2State.h"

CAttackLight2State::CAttackLight2State()
{
}

CAttackLight2State::~CAttackLight2State()
{
}

void CAttackLight2State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light2.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light2.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Light2.flip");
}

void CAttackLight2State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
	m_Weapon->Collider2D()->SetScale(Vec3(0.8f, 0.3f, 0.f));
	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);
}

void CAttackLight2State::FinalTick()
{
	Vec3 vGravityVelocity = m_Player->RigidBody()->GetVelocityByGravity();
	m_Player->RigidBody()->SetVelocityByGravity(vGravityVelocity * 0.5f);

	if (m_FBCom->GetFinish())
	{
		if (m_Player->RigidBody()->IsGround() == false)
			m_Player->FSM()->ChangeState(L"Fall");
		else
			m_Player->FSM()->ChangeState(L"Idle");
	}

	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();

	m_Weapon->Transform()->SetRelativeRotation(m_Player->Transform()->GetRelativeRotation());
	if (m_Player->Transform()->GetRelativeRotation().z > 0)
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x, WeaponPos.y - 10.f, WeaponPos.z));
	}

	else
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x, WeaponPos.y - 10.f, WeaponPos.z));
	}
}

void CAttackLight2State::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}


