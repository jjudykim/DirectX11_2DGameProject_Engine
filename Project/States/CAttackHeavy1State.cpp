#include "pch.h"
#include "CAttackHeavy1State.h"

CAttackHeavy1State::CAttackHeavy1State()
{
}

CAttackHeavy1State::~CAttackHeavy1State()
{
}


void CAttackHeavy1State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Heavy1.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Heavy1.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Heavy1.flip");
	
	m_AttackPower = 40;
	m_IsAttackState = true;
}

void CAttackHeavy1State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
	m_Weapon->Collider2D()->SetIndependentScale(true);
	m_Weapon->Collider2D()->SetScale(Vec3(0.55f, 0.4f, 0.f));
	m_Weapon->Collider2D()->SetOffset(Vec3(0.f, -0.06f, 0.f));
	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CAttackHeavy1State::FinalTick()
{
	if (m_FBCom->GetFinish())
	{
		m_Player->FSM()->ChangeState(L"Idle");
	}

	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();

	m_Weapon->Transform()->SetRelativeRotation(m_Player->Transform()->GetRelativeRotation());
	if (m_Player->Transform()->GetRelativeRotation().z > 0)
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x - 80.f, WeaponPos.y + 60.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(-50.f, 0.f, 0.f));
	}

	else
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x + 80.f, WeaponPos.y + 60.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(50.f, 0.f, 0.f));
	}
}

void CAttackHeavy1State::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	m_Weapon->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
}

