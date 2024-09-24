#include "pch.h"
#include "CBossScript.h"
#include "Engine\CLevelMgr.h"
#include "Engine\CLevel.h"
#include "Engine\CLogMgr.h"

// for random
#include <algorithm>
#include <random>
#include <array>

// Boss States
#include "States\CBossIdleState.h"
#include "States\CBossAttackState.h"
#include "States\CBossChainState.h"
#include "States\CBossFallState.h"
#include "States\CBossGroggyState.h"
#include "States\CBossJumpBackState.h"
#include "States\CBossDeadState.h"
#include "CMonsterScript.h"




CBossScript::CBossScript()
	: CScript(UINT(SCRIPT_TYPE::BOSSSCRIPT))
{
	AddScriptParam(SCRIPT_PARAM::INT, "BossHP", &m_BossHP);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "MonsterPref0", &m_MonsterPref0);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "MonsterPref1", &m_MonsterPref1);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "ProejctilePref0", &m_ArcProjectile);
	AddScriptParam(SCRIPT_PARAM::PREFAB, "ProjectilePref1", &m_BouncingProjectile);
	AddScriptParam(SCRIPT_PARAM::INT, "CurPhase", &m_CurPhase);
}

CBossScript::~CBossScript()
{
}

void CBossScript::Begin()
{
	m_CurPhase = 1;
	m_ShootProjectile = 0;
	m_CurPhaseStep = (UINT)PHASE_STEP::BEGIN;
	m_IsPuzzleOn = 0;
	m_PuzzleSize = 0.f;
	m_Time = 0.f;
	m_IsBossDead = false;

	m_MonsterSpawnState = 0;
	m_Player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_BossHP = 1500;
	m_DamageReactStandard = m_BossHP - (m_BossHP / 3);

	m_Puzzles[0].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle0");
	m_Puzzles[1].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle1");
	m_Puzzles[2].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle2");

	m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));
	m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));
	m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));

	FSM()->SetBlackboardData(L"CurPhase", DATA_TYPE::INT, &m_CurPhase);
	FSM()->SetBlackboardData(L"BossHP", DATA_TYPE::INT, &m_BossHP);
	FSM()->SetBlackboardData(L"PhaseStep", DATA_TYPE::INT, &m_CurPhaseStep);
	FSM()->SetBlackboardData(L"MonsterSpawnState", DATA_TYPE::INT, &m_MonsterSpawnState);
	FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);

	FSM()->AddState(L"BossIdle", new CBossIdleState);
	FSM()->AddState(L"BossAttack", new CBossAttackState);
	FSM()->AddState(L"BossChain", new CBossChainState);
	FSM()->AddState(L"BossFall", new CBossFallState);
	FSM()->AddState(L"BossGroggy", new CBossGroggyState);
	FSM()->AddState(L"BossJumpBack", new CBossJumpBackState);
	FSM()->AddState(L"BossDead", new CBossDeadState);

	FSM()->SetState();

	FSM()->ChangeState(L"BossIdle");
}

void CBossScript::Tick()
{
	FSM()->SetBlackboardData(L"PhaseStep", DATA_TYPE::INT, &m_CurPhaseStep);
	m_ShootProjectile = FSM()->GetBlackboardData<int>(L"ShootProjectile");

	if (m_ShootProjectile == 1)
	{
		CTimeMgr::GetInst()->AddTimer(0.7f, [this]() 
			{
				Vec3 pos = Transform()->GetWorldPos();
				Instantiate(m_ArcProjectile, 24, Vec3(pos.x - 30.f, pos.y, pos.z), L"ArcProjectile"); }, false);
		
		m_ShootProjectile = 0;
		FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);
	}
	else if (m_ShootProjectile == 2)
	{
		CTimeMgr::GetInst()->AddTimer(0.7f, [this]()
			{
				Vec3 pos = Transform()->GetWorldPos();
				Instantiate(m_BouncingProjectile, 24, Vec3(pos.x - 30.f, pos.y, pos.z), L"BouncingProjectile"); }, false);

		m_ShootProjectile = 0;
		FSM()->SetBlackboardData(L"ShootProjectile", DATA_TYPE::INT, &m_ShootProjectile);
	}

	if (KEY_TAP(KEY::CTRL))
	{
		FSM()->ChangeState(L"BossDead");
	}

	if (m_IsBossDead)
		FSM()->ChangeState(L"BossDead");

	if (m_CurPhaseStep == (UINT)PHASE_STEP::BEGIN)
	{
		if (!m_IsPuzzleOn && m_PuzzleState != 2)
		{
			InitAllPuzzles();
		}
		else if (m_IsPuzzleOn)
		{
			if (m_PuzzleState == 2)
			{
				m_CurPhaseStep = (UINT)PHASE_STEP::ATTACK;
				return;
			}

			for (int i = 0; i < 3; ++i)
			{
				if (!m_Puzzles[i].m_IsActivated && m_Puzzles[i].m_PuzzleObject->Collider2D()->GetOverlapCount() > 0)
				{
					m_Puzzles[i].m_IsActivated = true;
					m_IsAbleResetPuzzle = false;
					if (m_Puzzles[i].m_Priority != m_ActivePuzzleCount + 1)
					{
						m_PuzzleState = 1;
					}
					else
					{
						m_ActivePuzzleCount++;
					}
				}
				
				if (!m_IsAbleResetPuzzle)
				{
					if (m_Puzzles[i].m_IsActivated == true)
					{
						m_Puzzles[i].m_IsSetting = true;
						CGameObject* handle = m_Puzzles[i].m_PuzzleObject->GetChildByIndex(1);
						Vec3 vHandlePos = handle->Transform()->GetRelativePos();
						if (vHandlePos.y > -10.f)
						{
							handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, vHandlePos.y - (150.f * DT), 0.f));
						}
						else
						{
							handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, -10.f, 0.f));
							m_Puzzles[i].m_IsSetting = false;
						}

						if (m_Puzzles[0].m_IsSetting == false && m_Puzzles[1].m_IsSetting == false && m_Puzzles[2].m_IsSetting == false)
						{
							m_IsAbleResetPuzzle = true;
							if (m_Puzzles[0].m_IsActivated && m_Puzzles[1].m_IsActivated && m_Puzzles[2].m_IsActivated)
							{
								m_PuzzleState = 2;
							}
						}
					}
				}
				else
				{
					if (m_PuzzleState == 1 && m_IsAbleResetPuzzle)
						ResetPuzzles();
				}
			}
		}

		swprintf_s(m_PriorityInfo, L"[0] : %d, [1] : %d, [2] : %d", m_Puzzles[0].m_Priority, m_Puzzles[1].m_Priority, m_Puzzles[2].m_Priority);
		CLogMgr::GetInst()->AddLog({ m_PriorityInfo, 10, 40,  16, Vec4(255, 20, 20, 255) });
	}
	else if (m_CurPhaseStep == (UINT)PHASE_STEP::ATTACK)
	{
		if (m_IsPuzzleOn)
		{
			TermAllPuzzles();
		}

		if (m_BossHP < m_DamageReactStandard)
		{
			if (m_CurPhase == 3)
			{
				m_IsBossDead = true;
				return;
			}
				
			m_CurPhaseStep = (UINT)PHASE_STEP::SPAWN;
			m_MonsterCount = rand() % 10 + 4;
		}
	}
	else if (m_CurPhaseStep == (UINT)PHASE_STEP::SPAWN)
	{
		m_MonsterSpawnState = FSM()->GetBlackboardData<int>(L"MonsterSpawnState");

		if (m_MonsterSpawnState == 1)
		{
			CGameObject* SpawnPipe = CLevelMgr::GetInst()->FindObjectByName(L"SpawnPipe");

			Vec3 vPipePos = SpawnPipe->Transform()->GetRelativePos();

			if (vPipePos.y < 800.f)
			{
				SpawnPipe->Transform()->SetRelativePos(Vec3(vPipePos.x, 800.f, vPipePos.z));

				m_Time = 0.f;
				for (int i = 0; i < m_MonsterCount; ++i)
				{
					if (i % 2 == 0)
						SpawnMonsters(i, 0);
					else
						SpawnMonsters(i, 1);
				}

				m_MonsterSpawnState = 2;
				FSM()->SetBlackboardData(L"MonsterSpawnState", DATA_TYPE::INT, &m_MonsterSpawnState);
			}
			else
			{
				float PipePosY = vPipePos.y;
				PipePosY += sinf(m_Time * PI) * (600.f * DT * 2.f);

				SpawnPipe->Transform()->SetRelativePos(Vec3(vPipePos.x, PipePosY, vPipePos.z));
				m_Time += DT;
			}
		}
		else if (m_MonsterSpawnState == 2)
		{
			if (FSM()->GetCurState() == FSM()->FindState(L"BossChain")
				&& FlipBookComponent()->GetFinish())
			{
				FSM()->ChangeState(L"BossIdle");
			}

			if (m_vecSpawnedMonsters.empty())
			{
				CGameObject* pSpawnedMonster = nullptr;

				for (int i = 0; i < m_MonsterCount; ++i)
				{
					pSpawnedMonster = CLevelMgr::GetInst()->GetCurrentLevel()->FindObjectByName(L"Monster" + to_wstring(i));
					m_vecSpawnedMonsters.push_back(pSpawnedMonster);
				}
			}
			else
			{
				for (size_t i = 0; i < m_vecSpawnedMonsters.size(); ++i)
				{
					if (m_vecSpawnedMonsters[i]->IsDead())
					{
						m_vecSpawnedMonsters.erase(m_vecSpawnedMonsters.begin() + i);
						m_MonsterCount--;
					}
				}

				if (m_MonsterCount == 0)
				{
					m_MonsterSpawnState = 3;
					FSM()->SetBlackboardData(L"MonsterSpawnState", DATA_TYPE::INT, &m_MonsterSpawnState);
				}
			}
		}
		else if (m_MonsterSpawnState == 3)
		{
			CGameObject* SpawnPipe = CLevelMgr::GetInst()->FindObjectByName(L"SpawnPipe");

			Vec3 vPipePos = SpawnPipe->Transform()->GetRelativePos();

			if (vPipePos.y < 1400.f)
			{
				float PipePosY = vPipePos.y;
				PipePosY += sinf(m_Time * PI) * (800.f * DT * 2.f);

				SpawnPipe->Transform()->SetRelativePos(Vec3(vPipePos.x, PipePosY, vPipePos.z));
				m_Time += DT;
			}
			else
			{
				SpawnPipe->Transform()->SetRelativePos(Vec3(vPipePos.x, 1400.f, vPipePos.z));

				m_Time = 0.f;
				m_CurPhaseStep = (UINT)PHASE_STEP::END;
			}
			
		}
	}
	else if (m_CurPhaseStep == (UINT)PHASE_STEP::END)
	{
		m_CurPhaseStep = (UINT)PHASE_STEP::BEGIN;
		m_CurPhase++;

		if (m_CurPhase == 2) m_DamageReactStandard -= m_DamageReactStandard / 2;
		else if (m_CurPhase == 3) m_DamageReactStandard = 0;

		m_IsPuzzleOn = false;
		m_PuzzleState = 0;
		m_ActivePuzzleCount = 0;
		m_Time = 0.f;
		m_PuzzleSize = 0.f;
		m_IsAbleResetPuzzle = false;

		m_vecSpawnedMonsters.clear();
		m_MonsterSpawnState = 0;
		FSM()->SetBlackboardData(L"MonsterSpawnState", DATA_TYPE::INT, &m_MonsterSpawnState);
		m_MonsterCount = 0;
	}	
	
	swprintf_s(m_BossHPInfo, L"Current Boss HP : %d", m_BossHP);
	CLogMgr::GetInst()->AddLog({ m_BossHPInfo, 10, 60,  16, Vec4(255, 255, 20, 255) });
}

void CBossScript::SaveToFile(FILE* _File)
{
	fwrite(&m_BossHP, sizeof(int), 1, _File);
	SaveAssetRef(m_MonsterPref0, _File);
	SaveAssetRef(m_MonsterPref1, _File);
	SaveAssetRef(m_ArcProjectile, _File);
	SaveAssetRef(m_BouncingProjectile, _File);
}

void CBossScript::LoadFromFile(FILE* _File)
{
	fread(&m_BossHP, sizeof(int), 1, _File);
	LoadAssetRef(m_MonsterPref0, _File);
	LoadAssetRef(m_MonsterPref1, _File);
	LoadAssetRef(m_ArcProjectile, _File);
	LoadAssetRef(m_BouncingProjectile, _File);
}

void CBossScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
		return;

	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYERWEAPON)
	{
		DamagedByPlayerWeapon();
	}
}

void CBossScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (CLevelMgr::GetInst()->GetCurrentLevel()->GetState() != LEVEL_STATE::PLAY)
		return;

	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLAYERWEAPON)
	{
		if (m_Player->FSM()->GetCurState()->GetID() != m_PlayerAttackID)
		{
			DamagedByPlayerWeapon();
		}
	}
}

void CBossScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossScript::InitAllPuzzles()
{
	m_PuzzleSize += sinf(m_Time * PI) * (160.f * DT * 3.f);

	for (int i = 0; i < 3; ++i)
	{
		m_Puzzles[i].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));
		m_Puzzles[i].m_IsActivated = false;
		m_Puzzles[i].m_IsSetting = false;

		Vec3 vPuzzlePos = m_Puzzles[i].m_PuzzleObject->Transform()->GetRelativePos();
		m_Puzzles[i].m_PuzzleObject->Transform()->SetRelativePos(Vec3(vPuzzlePos.x, m_PuzzleSize / 2.f - 55.f, 0.f));
		vector<CGameObject*> vChildren = m_Puzzles[i].m_PuzzleObject->GetChildren();
		Vec3 vChildPos = vChildren[0]->Transform()->GetRelativePos();
		vChildren[0]->Transform()->SetRelativePos(Vec3(vChildPos.x, (m_PuzzleSize / 2.f) + 50.f, 0.f));
		vChildPos = vChildren[1]->Transform()->GetRelativePos();
		vChildren[1]->Transform()->SetRelativePos(Vec3(vChildPos.x, m_PuzzleSize / 2.f, 0.f));
	}

	m_Time += DT * 2.f;

	if (m_Time > 1.3f)
	{
		m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 0.f));
		m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 0.f));
		m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 150.f, 0.f));

		float fPuzzlePosx = m_Puzzles[0].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));
		fPuzzlePosx = m_Puzzles[1].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));
		fPuzzlePosx = m_Puzzles[2].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));

		m_IsPuzzleOn = true;
		m_Time = 0.f;
		PuzzlesPrioritySet();              // 우선 순위 설정
	}

}

void CBossScript::TermAllPuzzles()
{
	m_PuzzleSize -= sinf(m_Time * PI) * (160.f * DT * 3.f);

	m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));
	m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));
	m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));

	for (int i = 0; i < 3; ++i)
	{
		Vec3 vPuzzlePos = m_Puzzles[i].m_PuzzleObject->Transform()->GetRelativePos();
		m_Puzzles[i].m_PuzzleObject->Transform()->SetRelativePos(Vec3(vPuzzlePos.x, vPuzzlePos.y + (m_PuzzleSize / 2.f - 55.f), 0.f));

		vector<CGameObject*> vChildren = m_Puzzles[i].m_PuzzleObject->GetChildren();
		Vec3 vChildPos = vChildren[0]->Transform()->GetRelativePos();
		vChildren[0]->Transform()->SetRelativePos(Vec3(vChildPos.x, vPuzzlePos.y + ((m_PuzzleSize / 2.f) + 50.f), 0.f));
		vChildPos = vChildren[1]->Transform()->GetRelativePos();
		vChildren[1]->Transform()->SetRelativePos(Vec3(vChildPos.x, vPuzzlePos.y + (m_PuzzleSize / 2.f), 0.f));
	}

	m_Time += DT * 1.5f;

	if (m_Time > 1.3f)
	{
		m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
		m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
		m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));

		float fPuzzlePosx = m_Puzzles[0].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));
		fPuzzlePosx = m_Puzzles[1].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));
		fPuzzlePosx = m_Puzzles[2].m_PuzzleObject->Transform()->GetRelativePos().x;
		m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativePos(Vec3(fPuzzlePosx, 15.f, 0.f));

		m_IsPuzzleOn = false;
	}
}

void CBossScript::ResetPuzzles()
{
	for (int i = 0; i < 3; ++i)
	{
		CGameObject* handle = m_Puzzles[i].m_PuzzleObject->GetChildByIndex(1);
		Vec3 vHandlePos = handle->Transform()->GetRelativePos();

		if (m_Puzzles[i].m_IsActivated == true)
		{
			if (vHandlePos.y < 60.f)
				handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, vHandlePos.y + (150.f * DT), 0.f));
			else
			{
				handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, 60.f, 0.f));
				m_Puzzles[i].m_IsActivated = false;
			}
		}
	}

	if (m_Puzzles[0].m_IsActivated == false && m_Puzzles[1].m_IsActivated == false && m_Puzzles[2].m_IsActivated == false)
	{
		m_ActivePuzzleCount = 0;
		m_IsPuzzleOn = 3;
		m_PuzzleState = 0;
		m_IsAbleResetPuzzle = false;

		for (int i = 0; i < 3; ++i)
			m_Puzzles[i].m_PuzzleObject->Collider2D()->ResetOverlapCount();

		PuzzlesPrioritySet();
	}
}

void CBossScript::PuzzlesPrioritySet()
{
	std::array<int, 3> priorities = { 1, 2, 3 };
	
	random_device rd;
	mt19937 g(rd());
	
	shuffle(priorities.begin(), priorities.end(), g);
	
	for (int i = 0; i < 3; ++i)
		m_Puzzles[i].m_Priority = priorities[i];
}

void CBossScript::DamagedByPlayerWeapon()
{
	m_PlayerAttackID = m_Player->FSM()->GetCurState()->GetID();
	int iDamage = m_Player->FSM()->GetBlackboardData<int>(L"AttackPower");
	m_BossHP -= iDamage;

	//if (m_Alive && m_MonsterHP < 0)
	//{
	//	m_Alive = false;
	//	FSM()->SetBlackboardData(L"MonsterAlive", DATA_TYPE::INT, &m_Alive);
	//	MeshRender()->GetDynamicMaterial();
	//	MeshRender()->GetMaterial()->SetUseDeadEffect(true);
	//	CTimeMgr::GetInst()->AddTimer(2.0f, [this]()
	//		{
	//			CTaskMgr::GetInst()->AddTask({ TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)GetOwner() });
	//			//MeshRender()->GetMaterial()->SetUseDeadEffect(true);
	//		}, false);
	//	FSM()->ChangeState(L"ZombieDamage");
	//}
	//if (m_MonsterHP < m_DamageReactStandard)
	//{
	//	if (m_DamageReactStandard < m_MonsterHP * 0.4f)
	//		m_DamageReactStandard = 0;
	//	else
	//		m_DamageReactStandard = m_MonsterHP * 0.5f;
	//
	//	FSM()->ChangeState(L"ZombieDamage");
	//}
}

void CBossScript::SpawnMonsters(int _index, int _MonsterType)
{
	int RandPos = rand() % 100;

	if (_MonsterType == 0)
	{
		Instantiate(m_MonsterPref0, (int)LAYER_TYPE::MONSTER, Vec3(RandPos, 1000.f + (RandPos * 10), 0.f), L"Monster" + to_wstring(_index));
	}
	else
	{
		Instantiate(m_MonsterPref1, (int)LAYER_TYPE::MONSTER, Vec3(RandPos, 1000.f + (RandPos * 10), 0.f), L"Monster" + to_wstring(_index));
	}
		
}
