#include "pch.h"
#include "CAttackLight0State.h"

CAttackLight0State::CAttackLight0State()
{
}

CAttackLight0State::~CAttackLight0State()
{
}

void CAttackLight0State::Set()
{
	CPlayerState::Set();
	
	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light0.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light0.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Light0.flip");

	m_AttackPower = 20;
	m_IsAttackState = true;
}

void CAttackLight0State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
	m_Weapon->Collider2D()->SetIndependentScale(true);
	m_Weapon->Collider2D()->SetScale(Vec3(0.3f, 0.15f, 0.f));
	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_ConnectNextAttack = false;

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CAttackLight0State::FinalTick()
{
	Vec3 vGravityVelocity = m_Player->RigidBody()->GetVelocityByGravity();
	m_Player->RigidBody()->SetVelocityByGravity(vGravityVelocity * 0.5f);

	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();

	m_Weapon->Transform()->SetRelativeRotation(m_Player->Transform()->GetRelativeRotation());
	if (m_Player->Transform()->GetRelativeRotation().z > 0)
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x - 80.f, WeaponPos.y - 10.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(-50.f, 0.f, 0.f));
	}
	else
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x + 80.f, WeaponPos.y - 10.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(50.f, 0.f, 0.f));
	}

	if (KEY_TAP(KEY::A))
	{
		m_ConnectNextAttack = true;
	}

	if (KEY_TAP(KEY::D))
	{
		m_Player->FSM()->ChangeState(L"AttackUppercut");
	}

	if (m_FBCom->GetFinish())
	{
		if (m_ConnectNextAttack)
			m_Player->FSM()->ChangeState(L"AttackLight1");
		else
			if (m_Player->RigidBody()->IsGround() == false)
				m_Player->FSM()->ChangeState(L"Fall");
			else
				m_Player->FSM()->ChangeState(L"Idle");
	}
}

void CAttackLight0State::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}


