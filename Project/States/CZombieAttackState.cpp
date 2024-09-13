#include "pch.h"
#include "CZombieAttackState.h"

CZombieAttackState::CZombieAttackState()
{
}

CZombieAttackState::~CZombieAttackState()
{
}

void CZombieAttackState::Set()
{
	CMonsterState::Set();

	if (m_MonsterType == 1)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie0_Attack.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie0_Attack.flip");
	}
	else if (m_MonsterType == 2)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie1_Attack.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie1_Attack.flip");
	}

	m_RelativeScale = GetBlackboardData<Vec3>(L"OriginScale") * 1.2f;

	m_IsAttacking = true;
}

void CZombieAttackState::Enter()
{
	m_FBCom->Play(m_FBIdx, false);
	m_TargetDir = GetBlackboardData<UNITVEC_TYPE>(L"TargetDir");
	
	if (m_MonsterType == 1)
		m_Monster->Collider2D()->SetScale(Vec3(0.0f, 0.6f, 0.0f));
	else if (m_MonsterType == 2)
		m_Monster->Collider2D()->SetScale(Vec3(0.0f, 0.65f, 0.0f));

	m_Monster->Transform()->SetRelativeScale(m_RelativeScale);

	m_Speed = 3000.f;
	m_ProgressTime = 0.f;
	
	switch (m_TargetDir)
	{
	case UNITVEC_TYPE::LEFT:
	{
		m_Monster->RigidBody()->AddForce(Vec3(-m_Speed, 0.f, 0.f));
		if (m_MonsterType == 1)
			m_Monster->Collider2D()->SetOffset(Vec3(0.08f, -0.005f, 0.f));
		if (m_MonsterType == 2)
			m_Monster->Collider2D()->SetOffset(Vec3(0.08f, -0.01f, 0.f));
		break;
	}
	case UNITVEC_TYPE::RIGHT:
	{
		m_Monster->RigidBody()->AddForce(Vec3(m_Speed, 0.f, 0.f));
		
		if (m_MonsterType == 1)
			m_Monster->Collider2D()->SetOffset(Vec3(0.08f, -0.005f, 0.f));
		if (m_MonsterType == 2)
			m_Monster->Collider2D()->SetOffset(Vec3(0.08f, -0.01f, 0.f));
		break;
	}
	}

	m_Monster->FSM()->SetBlackboardData(L"IsMonsterAttackState", DATA_TYPE::INT, &m_IsAttacking);
}

void CZombieAttackState::FinalTick()
{
	m_IsLimitMap = GetBlackboardData<int>(L"LimitMap");

	if (m_IsLimitMap != 0)
	{
		m_Monster->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		if (m_TargetDir == UNITVEC_TYPE::LEFT)
			m_TargetDir = UNITVEC_TYPE::RIGHT;
		else
			m_TargetDir = UNITVEC_TYPE::LEFT;

		m_Monster->FSM()->SetBlackboardData(L"TargetDir", DATA_TYPE::UNITVEC_TYPE, &m_TargetDir);
	}
		
	float fCurSpeed = m_Speed * sin((m_ProgressTime / 1.f) * PI);
	float fColXSize = 0.3f * fabs(sin((m_ProgressTime / 1.f) * PI));

	m_ProgressTime += DT;

	if (m_MonsterType == 1)
		m_Monster->Collider2D()->SetScale(Vec3(fColXSize, 0.6f, 0.0f));
	else if (m_MonsterType == 2)
		m_Monster->Collider2D()->SetScale(Vec3(fColXSize, 0.65f, 0.0f));
	
	
	switch (m_TargetDir)
	{
	case UNITVEC_TYPE::LEFT:
		m_Monster->RigidBody()->AddForce(Vec3(-fCurSpeed, 0.f, 0.f));
		break;
	case UNITVEC_TYPE::RIGHT:
		m_Monster->RigidBody()->AddForce(Vec3(fCurSpeed, 0.f, 0.f));
		break;
	}

	if (m_FBCom->GetFinish())
	{
		m_Monster->FSM()->ChangeState(L"ZombieChase");
	}
}

void CZombieAttackState::Exit()
{
	m_Monster->Transform()->SetRelativeScale(GetBlackboardData<Vec3>(L"OriginScale"));
	m_Monster->Collider2D()->SetScale(Vec3(1.0f, 0.8f, 0.0f));
	m_Monster->Collider2D()->SetOffset(Vec3(0.08f, 0.005f, 0.f));
}