#include "pch.h"
#include "CAttackUppercutState.h"

CAttackUppercutState::CAttackUppercutState()
{
}

CAttackUppercutState::~CAttackUppercutState()
{
}

void CAttackUppercutState::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Uppercut.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Uppercut.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Uppercut.flip");
	
	m_AttackPower = 45;
	m_IsAttackState = true;
}

void CAttackUppercutState::Enter()
{
	m_OriginJumpSpeed = m_Player->RigidBody()->GetJumpSpeed();
	m_Player->RigidBody()->SetJumpSpeed(m_OriginJumpSpeed * 6.f);
	m_FBCom->Play(m_FBIdx, false);

	if (m_Player->Transform()->GetRelativeRotation().z > 0)
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		m_Weapon->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 180.f));
	}
	else
	{
		m_Dir = UNITVEC_TYPE::RIGHT;
		m_Weapon->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	}
	
	m_Player->RigidBody()->CurvedJump(true, m_Dir, DT);

	m_Weapon->Transform()->SetRelativeScale(Vec3(350.f, 350.f, 0.f));
	m_Weapon->Collider2D()->SetIndependentScale(true);
	m_Weapon->Collider2D()->SetScale(Vec3(0.5f, 0.5f, 0.f));

	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CAttackUppercutState::FinalTick()
{
	m_Player->RigidBody()->CurvedJump(false, m_Dir, DT);

	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();

	m_Weapon->Transform()->SetRelativeRotation(m_Player->Transform()->GetRelativeRotation());
	if (m_Player->Transform()->GetRelativeRotation().z > 0)
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x - 50.f, WeaponPos.y + 90.f, WeaponPos.z));
	else
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x + 50.f, WeaponPos.y + 90.f, WeaponPos.z));

	if (m_FBCom->GetFinish())
	{
		if (m_Player->RigidBody()->IsGround() == false)
		{
			m_Player->RigidBody()->SetVelocityByGravity(Vec3(0.f, 0.f, 0.f));
			m_Player->FSM()->ChangeState(L"Fall");
		}
		else
			m_Player->FSM()->ChangeState(L"Idle");
	}
}

void CAttackUppercutState::Exit()
{
	m_Player->RigidBody()->SetJumpSpeed(m_OriginJumpSpeed);
	m_Weapon->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	m_Weapon->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
}


