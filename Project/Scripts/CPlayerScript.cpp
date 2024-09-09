#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"

#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"

// Using States
#include "States/CIdleState.h"
#include "States/CRunState.h"
#include "States/CJumpState.h"
#include "States/CFallState.h"
#include "States/CDoubleJumpState.h"
#include "States/CDashState.h"
#include "States/CAttackLight0State.h"
#include "States/CAttackLight1State.h"
#include "States/CAttackLight2State.h"
#include "States/CAttackHeavy0State.h"
#include "States/CAttackHeavy1State.h"


CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
	, m_Dir(UNITVEC_TYPE::RIGHT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	m_Weapon = CLevelMgr::GetInst()->FindObjectByName(L"PlayerWeapon");
	m_GravityAccel = RigidBody()->GetGravityAccel();
	m_JumpSpeed = RigidBody()->GetJumpSpeed();
	m_Friction = RigidBody()->GetFriction();
	m_MaxWalkSpeed = RigidBody()->GetMaxWalkSpeed();
	RigidBody()->SetGroundDelegate(GetOwner(), (DELEGATE)&CPlayerScript::GroundLogic);
	RigidBody()->SetAirDelegate(GetOwner(), (DELEGATE)&CPlayerScript::AirLogic);

	FSM()->SetBlackboardData(L"Speed", DATA_TYPE::FLOAT, &m_Speed);
	FSM()->SetBlackboardData(L"JumpSpeed", DATA_TYPE::FLOAT, &m_JumpSpeed);
	FSM()->SetBlackboardData(L"Friction", DATA_TYPE::FLOAT, &m_Friction);
	FSM()->SetBlackboardData(L"MaxWalkSpeed", DATA_TYPE::FLOAT, &m_MaxWalkSpeed);
	FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	FSM()->SetBlackboardData(L"ReachNoPlatformCollider", DATA_TYPE::INT, &m_ReachNoPltCol);

	// FSM State
	FSM()->AddState(L"Idle", new CIdleState);
	FSM()->AddState(L"Run", new CRunState);
	FSM()->AddState(L"Jump", new CJumpState);
	FSM()->AddState(L"Fall", new CFallState);
	FSM()->AddState(L"DoubleJump", new CDoubleJumpState);
	FSM()->AddState(L"Dash", new CDashState);
	FSM()->AddState(L"AttackLight0", new CAttackLight0State);
	FSM()->AddState(L"AttackLight1", new CAttackLight1State);
	FSM()->AddState(L"AttackLight2", new CAttackLight2State);
	FSM()->AddState(L"AttackHeavy0", new CAttackHeavy0State);
	FSM()->AddState(L"AttackHeavy1", new CAttackHeavy1State);

	FSM()->SetState();

	FSM()->ChangeState(L"Idle");
}

void CPlayerScript::Tick()
{
	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	// m_ReachMapLimit (0 : 맵 끝 도달 X, 1 : 왼쪽 맵 끝 도달, 2 : 오른쪽 맵 끝 도달)

	if (m_OverlapPLTCount > 0)
		RigidBody()->SetGround(true);
	else
		RigidBody()->SetGround(false);
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsMapLimitObject(_OtherObject))
	{
		if (m_Dir == UNITVEC_TYPE::LEFT)
			m_ReachMapLimit = 1;
		else if (m_Dir == UNITVEC_TYPE::RIGHT)
			m_ReachMapLimit = 2;

		RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	}

	if (IsPlatformLayerObject(_OtherObject))
	{
		Vec3 OtherColPos = _OtherCollider->GetWorldPos();
		Vec3 OtherColScale = _OtherCollider->GetScale() * _OtherObject->Transform()->GetWorldScale();
		Vec3 OwnColPos = _OwnCollider->GetWorldPos();
		Vec3 OwnColScale = Collider2D()->GetScale() * GetOwner()->Transform()->GetWorldScale();

		Vec3 vVelocity = RigidBody()->GetVelocity() + RigidBody()->GetVelocityByGravity();
		Vec3 platformNormal = Vec3(0.0f, -1.0f, 0.0f);

		if (Vec3(0, 0, 0) != _OtherObject->Transform()->GetRelativeRotation())
		{
			Vec3 OtherRot = _OtherObject->Transform()->GetRelativeRotation() * _OtherObject->Transform()->GetWorldScale();
			Matrix matRot = XMMatrixRotationX(OtherRot.x) * XMMatrixRotationY(OtherRot.y) * XMMatrixRotationZ(OtherRot.z);

			// 플랫폼 법선을 회전 행렬로 변환
			platformNormal = XMVector3TransformNormal(platformNormal, matRot);
			platformNormal = platformNormal.Normalize();
		}

		XMVECTOR dotProductVector = XMVector3Dot(vVelocity.Normalize(), platformNormal);
		float dotProduct = XMVectorGetX(dotProductVector);

		float incidenceAngle = acosf(dotProduct);  // 입사각 (라디안 값)

		//Vec3 playerPosition = _OwnCollider->GetWorldPos();
		//Vec3 platformPosition = _OtherCollider->GetWorldPos();

		float fRelativeY = OwnColPos.y - OtherColPos.y;
		float fPlatformHeight = OtherColScale.y * 0.5f;

		// 플레이어가 플랫폼의 윗부분에 있는지 확인
		bool isAbovePlatform = fRelativeY > fPlatformHeight - 10.f;

		// 플레이어가 플랫폼의 아래쪽에서 충돌했는지 확인 
		bool isBelowPlatform = fRelativeY < -fPlatformHeight;

		// 입사각을 기준으로 충돌 처리
		float maxAngle;
		if (Vec3(0, 0, 0) != _OtherObject->Transform()->GetRelativeRotation())
			maxAngle = XMConvertToRadians(90.f);
		else
			maxAngle = XMConvertToRadians(85.f);

		if (isBelowPlatform)
		{
			RigidBody()->SetVelocityByGravity(Vec3(0.f, 0.f, 0.f));
		}
		
		if (incidenceAngle > maxAngle || !isAbovePlatform)
		{
			RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			if (vVelocity.x < 0) m_mapNonePlatform.insert(make_pair(_OtherCollider->GetID(), UNITVEC_TYPE::LEFT));
			else if (vVelocity.x > 0) m_mapNonePlatform.insert(make_pair(_OtherCollider->GetID(), UNITVEC_TYPE::RIGHT));
			else m_mapNonePlatform.insert(make_pair(_OtherCollider->GetID(), UNITVEC_TYPE::UP));
		}
		else
		{
			CorrectionYByFlatform(_OwnCollider, _OtherObject, _OtherCollider);
 			m_OverlapPLTCount++;
		}
	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		CorrectionYByFlatform(_OwnCollider, _OtherObject, _OtherCollider);

		map<UINT, UNITVEC_TYPE>::iterator iter = m_mapNonePlatform.begin();

		for (; iter != m_mapNonePlatform.end(); ++iter)
		{
			if (iter->first == _OtherCollider->GetID())
			{
				if (iter->second == UNITVEC_TYPE::LEFT)
					m_ReachNoPltCol = 1;
				else if (iter->second == UNITVEC_TYPE::RIGHT)
					m_ReachNoPltCol = 2;

				FSM()->SetBlackboardData(L"ReachNoPlatformCollider", DATA_TYPE::INT, &m_ReachNoPltCol);
			}
		}
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		map<UINT, UNITVEC_TYPE>::iterator iter = m_mapNonePlatform.begin();

		for (; iter != m_mapNonePlatform.end(); ++iter)
		{
			if (iter->first == _OtherCollider->GetID())
			{
				m_mapNonePlatform.erase(iter);
				m_ReachNoPltCol = 0;
				FSM()->SetBlackboardData(L"ReachNoPlatformCollider", DATA_TYPE::INT, &m_ReachNoPltCol);
				return;
			}
		}

		--m_OverlapPLTCount; 
	}

	if (IsMapLimitObject(_OtherObject))
	{ 
		m_ReachMapLimit = 0;
	}
}

void CPlayerScript::GroundLogic()
{
	RigidBody()->SetGravityAccel(0.f);
}

void CPlayerScript::AirLogic()
{
	RigidBody()->SetGravityAccel(m_GravityAccel);
}

void CPlayerScript::CorrectionYByFlatform(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	float Correction = 0;

	Vec3 OtherColPos = _OtherCollider->GetWorldPos();
	Vec3 OtherColScale = _OtherCollider->GetScale() * _OtherObject->Transform()->GetWorldScale();
	Vec3 OwnColPos = _OwnCollider->GetWorldPos();
	Vec3 OwnColScale = _OwnCollider->GetScale() * GetOwner()->Transform()->GetWorldScale();
	
	float playerCenterX = OwnColPos.x;
	float platformYAtPlayerX = OtherColPos.y;
	
	Matrix matRot = XMMatrixIdentity();

	if (Vec3(0, 0, 0) != _OtherObject->Transform()->GetRelativeRotation())
	{
		Vec3 OtherRot = _OtherObject->Transform()->GetRelativeRotation() * _OtherObject->Transform()->GetWorldScale();
		matRot = XMMatrixRotationX(OtherRot.x) * XMMatrixRotationY(OtherRot.y) * XMMatrixRotationZ(OtherRot.z);
		OtherColPos = XMVector3TransformCoord(OtherColPos, matRot);

		Vec3 tempPlayerCeneter = XMVector3TransformCoord(Vec3(playerCenterX, 0, 0), matRot);
		playerCenterX = tempPlayerCeneter.x;

		// *플랫폼의 충돌면에서의 플레이어의 X좌표와 충돌하는 부분 찾기*
		// : 플랫폼의 충돌 범위 내에서 플레이어의 X좌표가 위치하는 비율을 구함
		float platformMinX = OtherColPos.x - (OtherColScale.x / 2.f);  // 플랫폼의 좌측 경계
		float platformMaxX = OtherColPos.x + (OtherColScale.x / 2.f);  // 플랫폼의 우측 경계

		// 플레이어의 X좌표가 플랫폼 상에서 차지하는 비율 계산 (0.0 ~ 1.0 사이)
		float xRatio = (playerCenterX - platformMinX) / (platformMaxX - platformMinX);

		// 플랫폼의 Y좌표 계산
		float platformMinY = OtherColPos.y - (OtherColScale.y / 2.f);  // 플랫폼의 최저점 Y좌표
		float platformMaxY = OtherColPos.y + (OtherColScale.y / 2.f);  // 플랫폼의 최고점 Y좌표

		// 플레이어의 X좌표에 맞는 플랫폼 Y좌표를 보정 (Y축에서의 비율에 따라 보정할 수도 있음)
		platformYAtPlayerX = platformMinY + (platformMaxY - platformMinY) * xRatio;
	}

	float platformStandardY = platformYAtPlayerX + (OtherColScale.y / 2.f) - (OtherColScale.y * 0.1f);
	float playerBottomY = OwnColPos.y - (OwnColScale.y / 2.f);

	if (playerBottomY < platformStandardY) return;

	float Distance = fabs(platformYAtPlayerX - OwnColPos.y);
	float Standard = (OtherColScale.y / 2.f) + (OwnColScale.y / 2.f);

	if (Distance < Standard)
	{
		Correction = Standard - Distance;
	}

	Vec3 vPos = Transform()->GetRelativePos();
	vPos.y += Correction;

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	SaveAssetRef(m_Texture, _File);
	SaveAssetRef(m_MissilePref, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	LoadAssetRef(m_Texture, _File);
	LoadAssetRef(m_MissilePref, _File);
}