#include "pch.h"
#include "CZombieChaseState.h"

#include "Engine\CLevelMgr.h"

CZombieChaseState::CZombieChaseState()
{
}

CZombieChaseState::~CZombieChaseState()
{
}

void CZombieChaseState::Set()
{
	CMonsterState::Set();

	if (m_MonsterType == 1)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie0_Chase.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie0_Chase.flip");
	}
	else if (m_MonsterType == 2)
	{
		m_CurFB = m_FBCom->FindFlipBook(L"animation\\Zombie1_Chase.flip");
		m_FBIdx = m_FBCom->FindFlipBookIndex(L"animation\\Zombie1_Chase.flip");
	}

	m_IsAttacking = false;
}

void CZombieChaseState::Enter()
{
	m_FBCom->Play(m_FBIdx, true);

	if (m_MonsterType == 1)
		m_Monster->Collider2D()->SetScale(Vec3(1.1f, 0.8f, 0.0f));
	else if (m_MonsterType == 2)
		m_Monster->Collider2D()->SetScale(Vec3(1.1f, 0.9f, 0.0f));

	CGameObject* player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	Vec3 vPlayerPos = player->Transform()->GetRelativePos();

	if (m_Monster->Transform()->GetRelativePos().x - vPlayerPos.x > 0)
	{
		m_TargetDir = UNITVEC_TYPE::LEFT;
		m_Monster->FSM()->SetBlackboardData(L"TargetDir", DATA_TYPE::UNITVEC_TYPE, &m_TargetDir);
	}
		
	else if (m_Monster->Transform()->GetRelativePos().x - vPlayerPos.x <= 0)
	{
		m_TargetDir = UNITVEC_TYPE::RIGHT;
		m_Monster->FSM()->SetBlackboardData(L"TargetDir", DATA_TYPE::UNITVEC_TYPE, &m_TargetDir);
	}

	int bAlive = GetBlackboardData<int>(L"MonsterAlive");
	if (bAlive == 0)
		m_Monster->Collider2D()->SetScale(Vec3(0.f, 0.f, 0.f));

	m_Monster->FSM()->SetBlackboardData(L"IsMonsterAttackState", DATA_TYPE::INT, &m_IsAttacking);
}

void CZombieChaseState::FinalTick()
{
	m_IsLimitMap = GetBlackboardData<int>(L"LimitMap");

	if (m_IsLimitMap != 0)
	{
		switch (m_TargetDir)
		{
		case UNITVEC_TYPE::LEFT:
			m_TargetDir = UNITVEC_TYPE::RIGHT;
			break;

		case UNITVEC_TYPE::RIGHT:
			m_TargetDir = UNITVEC_TYPE::LEFT;
			break;
		}

		m_IsLimitMap = 0;
		m_Monster->FSM()->SetBlackboardData(L"LimitMap", DATA_TYPE::INT, &m_IsLimitMap);
	}

	Vec3 vOriginPos = m_Monster->Transform()->GetRelativePos();
	m_Speed = 200.f * DT;

	switch (m_TargetDir)
	{
	case UNITVEC_TYPE::LEFT:
		m_Monster->Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		m_Monster->Transform()->SetRelativePos(Vec3(vOriginPos.x - m_Speed, vOriginPos.y, vOriginPos.z));
		break;

	case UNITVEC_TYPE::RIGHT:
		m_Monster->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		m_Monster->Transform()->SetRelativePos(Vec3(vOriginPos.x + m_Speed, vOriginPos.y, vOriginPos.z));
		break;
	}

	if (GetBlackboardData<int>(L"OverlapWithPlayer") == 1)
	{
		m_OverlapWithPlayer = 0;
		m_Monster->FSM()->SetBlackboardData(L"OverlapWithPlayer", DATA_TYPE::INT, &m_OverlapWithPlayer);
		m_Monster->FSM()->ChangeState(L"ZombieAttack");
	}
}

void CZombieChaseState::Exit()
{
}