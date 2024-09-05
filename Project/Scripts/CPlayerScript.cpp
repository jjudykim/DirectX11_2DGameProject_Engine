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


CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
	, m_JumpCount(0.f)
	, m_Dir(UNITVEC_TYPE::RIGHT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	//GetRenderComponent()->GetDynamicMaterial();
	m_GravityAccel = RigidBody()->GetGravityAccel();
	RigidBody()->SetGroundDelegate(GetOwner(), (DELEGATE)&CPlayerScript::GroundLogic);
	RigidBody()->SetAirDelegate(GetOwner(), (DELEGATE)&CPlayerScript::AirLogic);

	FSM()->SetBlackboardData(L"Speed", DATA_TYPE::FLOAT, &m_Speed);
	FSM()->SetBlackboardData(L"JumpCount", DATA_TYPE::INT, &m_JumpCount);
	FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);

	// FSM State
	FSM()->AddState(L"Idle", new CIdleState);
	FSM()->AddState(L"Run", new CRunState);
	FSM()->AddState(L"Jump", new CJumpState);
	FSM()->AddState(L"Fall", new CFallState);
	FSM()->AddState(L"DoubleJump", new CDoubleJumpState);

	FSM()->SetState();

	FSM()->ChangeState(L"Idle");
}

void CPlayerScript::Tick()
{
	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	// m_ReachMapLimit (0 : �� �� ���� X, 1 : ���� �� �� ����, 2 : ������ �� �� ����)

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
	}

	if (IsPlatformLayerObject(_OtherObject))
	{
		CorrectionYByFlatform(_OwnCollider, _OtherObject, _OtherCollider);

		Vec3 OtherColPos = _OtherCollider->GetWorldPos();
		Vec3 OtherColScale = _OtherCollider->GetScale() * _OtherObject->Transform()->GetWorldScale();
		Vec3 OwnColPos = Transform()->GetWorldPos();
		Vec3 OwnColScale = Collider2D()->GetScale() * GetOwner()->Transform()->GetWorldScale();

		float playerBottomY = OwnColPos.y - (OwnColScale.y / 2.f);

		if (playerBottomY < OtherColPos.y)
		{
			m_vecNonePlatform.push_back(_OtherCollider);
			RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			return;
		}

		Vec3 vVelocity = RigidBody()->GetVelocity();
		Vec3 platformNormal = Vec3(0.0f, 1.0f, 0.0f);

		if (Vec3(0, 0, 0) != _OtherObject->Transform()->GetRelativeRotation())
		{
			Vec3 OtherRot = _OtherObject->Transform()->GetRelativeRotation() * _OtherObject->Transform()->GetWorldScale();
			Matrix matRot = XMMatrixRotationX(OtherRot.x) * XMMatrixRotationY(OtherRot.y) * XMMatrixRotationZ(OtherRot.z);

			// �÷��� ������ ȸ�� ��ķ� ��ȯ
			platformNormal = XMVector3TransformNormal(platformNormal, matRot);
			platformNormal = platformNormal.Normalize();
		}

		XMVECTOR dotProductVector = XMVector3Dot(vVelocity.Normalize(), platformNormal);
		float dotProduct = XMVectorGetX(dotProductVector); 
		
		float incidenceAngle = acosf(dotProduct);  // �Ի簢 (���� ��)
		
		// �Ի簢�� �������� �浹 ó��
		float maxAngle = XMConvertToRadians(90.0f);
		if (incidenceAngle > maxAngle)
		{
			m_vecNonePlatform.push_back(_OtherCollider);
			RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		}
		else
		{
			m_OverlapPLTCount++;
		}
	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		CorrectionYByFlatform(_OwnCollider, _OtherObject, _OtherCollider);
	}
}

void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		vector<CCollider2D*>::iterator iter = m_vecNonePlatform.begin();

		for (; iter != m_vecNonePlatform.end(); ++iter)
		{
			if ((*iter)->GetID() == _OtherCollider->GetID())
			{
				m_vecNonePlatform.erase(iter);
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

		// *�÷����� �浹�鿡���� �÷��̾��� X��ǥ�� �浹�ϴ� �κ� ã��*
		// : �÷����� �浹 ���� ������ �÷��̾��� X��ǥ�� ��ġ�ϴ� ������ ����
		float platformMinX = OtherColPos.x - (OtherColScale.x / 2.f);  // �÷����� ���� ���
		float platformMaxX = OtherColPos.x + (OtherColScale.x / 2.f);  // �÷����� ���� ���

		// �÷��̾��� X��ǥ�� �÷��� �󿡼� �����ϴ� ���� ��� (0.0 ~ 1.0 ����)
		float xRatio = (playerCenterX - platformMinX) / (platformMaxX - platformMinX);

		// �÷����� Y��ǥ ���
		float platformMinY = OtherColPos.y - (OtherColScale.y / 2.f);  // �÷����� ������ Y��ǥ
		float platformMaxY = OtherColPos.y + (OtherColScale.y / 2.f);  // �÷����� �ְ��� Y��ǥ

		// �÷��̾��� X��ǥ�� �´� �÷��� Y��ǥ�� ���� (Y�࿡���� ������ ���� ������ ���� ����)
		platformYAtPlayerX = platformMinY + (platformMaxY - platformMinY) * xRatio;
	}

	float platformCenterY = platformYAtPlayerX;
	float playerBottomY = OwnColPos.y - (OwnColScale.y / 2.f);

	if (playerBottomY < platformCenterY) return;

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