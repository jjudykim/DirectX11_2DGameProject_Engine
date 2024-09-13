#include "pch.h"
#include "CAttackLight1State.h"

CAttackLight1State::CAttackLight1State()
{
}

CAttackLight1State::~CAttackLight1State()
{
}

void CAttackLight1State::Set()
{
	CPlayerState::Set();

	m_CurFB = m_FBCom->FindFlipBook(L"animation\\Scary_Attack_Light1.flip");
	m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Scary_Attack_Light1.flip");
	m_WeaponFBIdx = m_Weapon->FlipBookComponent()->FindFlipBookIndex(L"animation\\Scary_Weapon_Light1.flip");
	
	m_AttackPower = 25;
	m_IsAttackState = true;
}

void CAttackLight1State::Enter()
{
	m_FBCom->Play(m_FBIdx, false);

	m_Weapon->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
	m_Weapon->Collider2D()->SetScale(Vec3(0.5f, 0.25f, 0.f));
	m_Weapon->FlipBookComponent()->Play(m_WeaponFBIdx, false);

	m_ConnectNextAttack = false;

	m_Player->FSM()->SetBlackboardData(L"AttackPower", DATA_TYPE::INT, &m_AttackPower);
}

void CAttackLight1State::FinalTick()
{
	Vec3 vGravityVelocity = m_Player->RigidBody()->GetVelocityByGravity();
	m_Player->RigidBody()->SetVelocityByGravity(vGravityVelocity * 0.5f);

	if (KEY_TAP(KEY::A))
	{
		m_ConnectNextAttack = true;
	}

	if (m_FBCom->GetFinish())
	{
		if (m_ConnectNextAttack)
			m_Player->FSM()->ChangeState(L"AttackLight2");
		else
			if (m_Player->RigidBody()->IsGround() == false)
				m_Player->FSM()->ChangeState(L"Fall");
			else
				m_Player->FSM()->ChangeState(L"Idle");
	}

	Vec3 WeaponPos = m_Player->Transform()->GetRelativePos();

	m_Weapon->Transform()->SetRelativeRotation(m_Player->Transform()->GetRelativeRotation());
	if (m_Player->Transform()->GetRelativeRotation().z > 0)
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x + 40.f, WeaponPos.y - 10.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(-50.f, 0.f, 0.f));
	}

	else
	{
		m_Weapon->Transform()->SetRelativePos(Vec3(WeaponPos.x - 40.f, WeaponPos.y - 10.f, WeaponPos.z));
		m_Player->RigidBody()->AddForce(Vec3(50.f, 0.f, 0.f));
	}
}

void CAttackLight1State::Exit()
{
	m_Weapon->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}


