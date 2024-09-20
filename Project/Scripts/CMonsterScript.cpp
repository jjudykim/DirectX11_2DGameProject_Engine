#include "pch.h"
#include "CMonsterScript.h"

#include "Engine\CLevelMgr.h"

// Zombie States
#include "States\CPlayerState.h"
#include "States\CZombieIdleState.h"
#include "States\CZombieChaseState.h"
#include "States\CZombieAttackState.h"
#include "States\CZombieDamageState.h"

CMonsterScript::CMonsterScript()
	: CScript(UINT(SCRIPT_TYPE::MONSTERSCRIPT))
	, m_MonsterType(MONSTER_TYPE::NONE)
	, m_Alive(true)
{
	// 0 : None
	// 1 : Zombie0
	// 2 : Zombie1
	// 3 : ZombieDog
	// 4 : ZombieBat
	// 5 : Franken
	// 6 : Boomer
	AddScriptParam(SCRIPT_PARAM::INT, "MonsterType", &m_MonsterType);
	AddScriptParam(SCRIPT_PARAM::INT, "MonsterHP", &m_MonsterHP);
	AddScriptParam(SCRIPT_PARAM::INT, "MonsterAttackPower", &m_MonsterAttackPower);
}

CMonsterScript::~CMonsterScript()
{
}

void CMonsterScript::Begin()
{
	m_OverlapWithPlayer = 0;
	m_IsLimitMap = 0;
	m_OriginScale = Transform()->GetRelativeScale();
	m_Player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_DamageReactStandard = m_MonsterHP * 0.7;

	FSM()->SetBlackboardData(L"MonsterType", DATA_TYPE::INT, &m_MonsterType);
	FSM()->SetBlackboardData(L"OverlapWithPlayer", DATA_TYPE::INT, &m_OverlapWithPlayer);
	FSM()->SetBlackboardData(L"OriginScale", DATA_TYPE::VEC3, &m_OriginScale);
	FSM()->SetBlackboardData(L"LimitMap", DATA_TYPE::INT, &m_IsLimitMap);
	FSM()->SetBlackboardData(L"MonsterHP", DATA_TYPE::INT, &m_MonsterHP);
	FSM()->SetBlackboardData(L"MonsterAttackPower", DATA_TYPE::INT, &m_MonsterAttackPower);
	FSM()->SetBlackboardData(L"MonsterAlive", DATA_TYPE::INT, &m_Alive);
	FSM()->SetBlackboardData(L"IsMonsterAttackState", DATA_TYPE::INT, &m_IsAttacking);

	switch (m_MonsterType)
	{
	case MONSTER_TYPE::ZOMBIE0:
	case MONSTER_TYPE::ZOMBIE1:
	{
		FSM()->AddState(L"ZombieIdle", new CZombieIdleState);
		FSM()->AddState(L"ZombieChase", new CZombieChaseState);
		FSM()->AddState(L"ZombieAttack", new CZombieAttackState);
		FSM()->AddState(L"ZombieDamage", new CZombieDamageState);

		FSM()->SetState();

		FSM()->ChangeState(L"ZombieIdle");
		break;
	}

	case MONSTER_TYPE::ZOMBIEDOG:
	{
		break;
	}
	case MONSTER_TYPE::ZOMBIEBAT:
	{
		break;
	}
	case MONSTER_TYPE::FRANKEN:
	{
		break;
	}
	case MONSTER_TYPE::BOOMER:
	{
		break;
	}
	}
}

void CMonsterScript::Tick()
{
	if (m_OverlapPLTCount > 0)
		RigidBody()->SetGround(true);
	else
		RigidBody()->SetGround(false);
}

void CMonsterScript::SaveToFile(FILE* _File)
{
	fwrite(&m_MonsterType, sizeof(UINT), 1, _File);
	fwrite(&m_MonsterHP, sizeof(int), 1, _File);
	fwrite(&m_MonsterAttackPower, sizeof(int), 1, _File);
}

void CMonsterScript::LoadFromFile(FILE* _File)
{
	fread(&m_MonsterType, sizeof(UINT), 1, _File);
	fread(&m_MonsterHP, sizeof(int), 1, _File);
	fread(&m_MonsterAttackPower, sizeof(int), 1, _File);
}

void CMonsterScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		if (_OtherCollider->GetWorldPos().y < _OwnCollider->GetWorldPos().y)
		{
			++m_OverlapPLTCount;
		}
		else
		{
			m_IsLimitMap = 1;
			FSM()->SetBlackboardData(L"LimitMap", DATA_TYPE::INT, &m_IsLimitMap);
		}
	}

	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYER)
	{
		m_OverlapWithPlayer = 1;
		FSM()->SetBlackboardData(L"OverlapWithPlayer", DATA_TYPE::INT, &m_OverlapWithPlayer);
	}

	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYERWEAPON)
	{
		DamagedByPlayerWeapon();
	}
}

void CMonsterScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYERWEAPON)
	{
		if (m_Player->FSM()->GetCurState()->GetID() != m_PlayerAttackID)
		{
			DamagedByPlayerWeapon();
		}
	}
}

void CMonsterScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		if (_OtherCollider->GetWorldPos().y < _OwnCollider->GetWorldPos().y)
		{
			--m_OverlapPLTCount;
		}
		else
		{
			m_IsLimitMap = 0;
			FSM()->SetBlackboardData(L"LimitMap", DATA_TYPE::INT, &m_IsLimitMap);
		}
	}
}

void CMonsterScript::DamagedByPlayerWeapon()
{
	m_PlayerAttackID = m_Player->FSM()->GetCurState()->GetID();
	int iDamage = m_Player->FSM()->GetBlackboardData<int>(L"AttackPower");
	m_MonsterHP -= iDamage;

	if (m_Alive && m_MonsterHP < 0)
	{
		m_Alive = false;
		FSM()->SetBlackboardData(L"MonsterAlive", DATA_TYPE::INT, &m_Alive);
		MeshRender()->GetDynamicMaterial();
		MeshRender()->GetMaterial()->SetUseDeadEffect(true);
		CTimeMgr::GetInst()->AddTimer(2.0f, [this]()
			{
				CTaskMgr::GetInst()->AddTask({ TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)GetOwner() });
				//MeshRender()->GetMaterial()->SetUseDeadEffect(true);
			}, false);
		FSM()->ChangeState(L"ZombieDamage");
	}
	if (m_MonsterHP < m_DamageReactStandard)
	{
		if (m_DamageReactStandard < m_MonsterHP * 0.4f)
			m_DamageReactStandard = 0;
		else
			m_DamageReactStandard = m_MonsterHP * 0.5f;

		FSM()->ChangeState(L"ZombieDamage");
	}
}