#include "pch.h"
#include "CBossScript.h"
#include "Engine\CLevelMgr.h"

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



CBossScript::CBossScript()
	: CScript(UINT(SCRIPT_TYPE::BOSSSCRIPT))
{
}

CBossScript::~CBossScript()
{
}

void CBossScript::Begin()
{
	AddScriptParam(SCRIPT_PARAM::INT, "BossHP", &m_BossHP);
	AddScriptParam(SCRIPT_PARAM::INT, "CurPhase", &m_CurPhase);

	m_CurPhase = 1;
	m_CurPhaseStep = PHASE_STEP::BEGIN;
	m_IsPuzzleOn = 0;
	m_PuzzleSize = 0.f;
	m_Time = 0.f;

	m_Puzzles[0].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle0");
	m_Puzzles[1].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle1");
	m_Puzzles[2].m_PuzzleObject = CLevelMgr::GetInst()->FindObjectByName(L"TriggerPuzzle2");

	m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));
	m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));
	m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, 0.f, 0.f));

	FSM()->SetBlackboardData(L"CurPhase", DATA_TYPE::INT, &m_CurPhase);
	FSM()->SetBlackboardData(L"BossHP", DATA_TYPE::INT, &m_BossHP);
	FSM()->SetBlackboardData(L"IsPuzzleOn", DATA_TYPE::INT, &m_IsPuzzleOn);
	FSM()->SetBlackboardData(L"PhaseStep", DATA_TYPE::INT, &m_CurPhaseStep);

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
	if (KEY_TAP(KEY::CTRL))
	{
		FSM()->ChangeState(L"BossFall");
	}

	if (m_CurPhaseStep == PHASE_STEP::BEGIN)
	{
		if (m_IsPuzzleOn == 0)
		{
			InitAllPuzzles();
		}
		else if (m_IsPuzzleOn > 0)
		{
			for (int i = 0; i < 3; ++i)
			{
				if (!m_Puzzles[i].m_IsActivated && m_Puzzles[i].m_PuzzleObject->Collider2D()->GetOverlapCount() > 0)
				{
					m_Puzzles[i].m_IsActivated = true;
					m_ActivePuzzleCount++;
					if (m_Puzzles[i].m_Priority != m_ActivePuzzleCount)
					{
						m_IsPuzzleFailed = true;
					}
				}
				if (!m_ResettingPuzzle)
				{
					if (m_Puzzles[i].m_IsActivated == true)
					{
						CGameObject* handle = m_Puzzles[i].m_PuzzleObject->GetChildByIndex(1);
						Vec3 vHandlePos = handle->Transform()->GetRelativePos();
						if (vHandlePos.y > -10.f)
						{
							handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, vHandlePos.y - (150.f * DT), 0.f));
						}
						else
						{
							handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, -10.f, 0.f));
						}
					}
				}
			}
		}
	}
	else if (m_CurPhaseStep == PHASE_STEP::ATTACK)
	{
		

	}
}

void CBossScript::SaveToFile(FILE* _File)
{
}

void CBossScript::LoadFromFile(FILE* _File)
{
}

void CBossScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	
}

void CBossScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CBossScript::InitAllPuzzles()
{
	m_PuzzleSize += sinf(m_Time * PI) * (160.f * DT * 3.f);

	m_Puzzles[0].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));
	m_Puzzles[1].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));
	m_Puzzles[2].m_PuzzleObject->Transform()->SetRelativeScale(Vec3(150.f, m_PuzzleSize, 0.f));

	for (int i = 0; i < 3; ++i)
	{
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

		m_IsPuzzleOn = 3;
		m_Time = 0.f;
		PuzzlesPrioritySet();              // 우선 순위 설정
	}

}

void CBossScript::ResetPuzzles()
{
	for (int i = 0; i < 3; ++i)
	{
		CGameObject* handle = m_Puzzles[i].m_PuzzleObject->GetChildByIndex(1);
		Vec3 vHandlePos = handle->Transform()->GetRelativePos();

		if (vHandlePos.y < 60.f)
			handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, vHandlePos.y + (150.f * DT), 0.f));
		else
		{
			handle->Transform()->SetRelativePos(Vec3(vHandlePos.x, 60.f, 0.f));
			m_Puzzles[i].m_IsActivated = false;
		}
	}

	if (m_Puzzles[0].m_IsActivated == false && m_Puzzles[1].m_IsActivated == false && m_Puzzles[2].m_IsActivated == false)
	{
		m_ActivePuzzleCount = 0;
		m_IsPuzzleOn = 3;
		m_IsPuzzleFailed = false;
		m_ResettingPuzzle = false;

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
