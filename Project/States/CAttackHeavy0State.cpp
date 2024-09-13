#include "pch.h"
#include "CAttackHeavy0State.h"

CAttackHeavy0State::CAttackHeavy0State()
{
}

CAttackHeavy0State::~CAttackHeavy0State()
{
}

void CAttackHeavy0State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Heavy0.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Heavy0.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Heavy0.flip");
	
	m_AttackPower = 30;
	m_IsAttackState = true;
}

void CAttackHeavy0State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
	m_Weapon->Collider2D()->SetIndependentScale(true);
	m_Weapon->Collider2D()->SetScale(Vec3(0.55f, 0.4f, 0.f));
	m_Weapon->Collider2D()->SetOffset(Vec3(0.f, -0.06f, 0.f));
	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_ConnectNextAttack = false;

	if (m_Player->RigidBody()->IsGround() == false)
	{
		float vGravityVelocityY = m_Player->RigidBody()->GetVelocityByGravity().y;
		m_Player->RigidBody()->SetVelocityByGravity(Vec3(0.f, vGravityVelocityY - 300.f,  0.f));
	}

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CAttackHeavy0State::FinalTick()
{
	if (KEY_TAP(KEY::D))
	{
		m_ConnectNextAttack = true;
	}

	if (m_FBCom->GetFinish())
	{
		if (m_ConnectNextAttack)
			m_Player->FSM()->ChangeState(L"AttackHeavy1");
		else
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

void CAttackHeavy0State::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	m_Weapon->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
}


