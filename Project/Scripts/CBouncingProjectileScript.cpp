#include "pch.h"
#include "CBouncingProjectileScript.h"

#include "Engine\CLevelMgr.h"

CBouncingProjectileScript::CBouncingProjectileScript()
	: CScript(SCRIPT_TYPE::BOUNCINGPROJECTILESCRIPT)
{
	m_IsMaxSize = false;
	m_IsExploding = false;
}

CBouncingProjectileScript::~CBouncingProjectileScript()
{
}


void CBouncingProjectileScript::Begin()
{
	m_Player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_IsExploding = false;

	Vec3 vPlayerPos = m_Player->Transform()->GetRelativePos();
	m_DestinationPosX = Transform()->GetRelativePos().x - vPlayerPos.x;

	RigidBody()->AddVelocityByGravity(Vec3(0.f, 600.f, 0.f));

	RigidBody()->SetMaxWalkSpeed(1200.f);
}

void CBouncingProjectileScript::Tick()
{
	if (!m_IsExploding)
	{
		Vec3 vCurRelativeScale = Transform()->GetRelativeScale();
		Transform()->SetRelativeScale(vCurRelativeScale * 1.0003f);
		if (Transform()->GetRelativeScale().x > 80.f)
		{
			m_IsMaxSize = true;
		}

		m_VelocityX = m_DestinationPosX * (-3.f);

		RigidBody()->AddForce(Vec3(m_VelocityX, 0.f, 0.f));
	}
	else
	{
		CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)GetOwner() });
	}
}

void CBouncingProjectileScript::SaveToFile(FILE* _File)
{
}

void CBouncingProjectileScript::LoadFromFile(FILE* _File)
{
}

void CBouncingProjectileScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (_OtherObject->GetLayerIdx() == (int)LAYER_TYPE::PLATFORM3)
	{
		Vec3 vIncidentVec = RigidBody()->GetVelocity();
		Vec3 vPlatformNormal;

		Vec3 OwnScale = Transform()->GetRelativeScale() * _OwnCollider->GetScale();
		float OwnTopSide = _OwnCollider->GetWorldPos().y + (OwnScale.y / 2.f);
		float OwnBottomSide = _OwnCollider->GetWorldPos().y - (OwnScale.y / 2.f);
		float OwnLeftSide = _OwnCollider->GetWorldPos().x - (OwnScale.x / 2.f);
		float OwnRightSide = _OwnCollider->GetWorldPos().x + (OwnScale.x / 2.f);

		Vec3 PlatformScale = _OtherObject->Transform()->GetRelativeScale() * _OtherCollider->GetScale();
		float PlatformTopSide = _OtherCollider->GetWorldPos().y + (PlatformScale.y / 2.f);
		float PlatformBottomSide = _OtherCollider->GetWorldPos().y - (PlatformScale.y / 2.f);
		float PlatformLeftSide = _OtherCollider->GetWorldPos().x - (PlatformScale.x / 2.f);
		float PlatformRightSide = _OtherCollider->GetWorldPos().x + (PlatformScale.x / 2.f);

		Vec3 vReflectionVec;

		if (OwnLeftSide - PlatformRightSide > - 10.f)
		{
			// Transform()->GetRelativePos().x  && PlatformRightSide >= OwnLeftSide
			RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			vPlatformNormal = Vec3(0.f, -1.0f, 0.0f);

			vReflectionVec = vIncidentVec - 2.f * vIncidentVec.Dot(vPlatformNormal) * vPlatformNormal;
		}
		else if (OwnRightSide - PlatformLeftSide < 10.f)
		{
			// RigidBody()->GetVelocity().x > 0.f && PlatformLeftSide <= OwnRightSide
			RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			vPlatformNormal = Vec3(0.f,  -1.0f, 0.0f);

			vReflectionVec = vIncidentVec - 2.f * vIncidentVec.Dot(vPlatformNormal) * vPlatformNormal;
		}
		else if (RigidBody()->GetVelocityByGravity().y < 0.f && PlatformTopSide >= OwnBottomSide)
		{
			if (m_IsMaxSize)
			{
				m_IsExploding = true;
				return;
			}
			vPlatformNormal = Vec3(0.0f, -1.0f, 0.0f);
			RigidBody()->AddVelocityByGravity(Vec3(0.f, 0.f, 0.f));
			RigidBody()->AddVelocityByGravity(Vec3(0.f, 1200.f, 0.f));

			vReflectionVec = vIncidentVec - 2.f * vIncidentVec.Dot(vPlatformNormal) * vPlatformNormal;
			vReflectionVec *= -1;
		}
		else if (RigidBody()->GetVelocityByGravity().y > 0.f && PlatformBottomSide <= OwnTopSide)
		{
			vPlatformNormal = Vec3(0.0f, 1.0f, 0.0f);
			RigidBody()->AddVelocityByGravity(Vec3(0.f, 0.f, 0.f));

			vReflectionVec = vIncidentVec - 2.f * vIncidentVec.Dot(vPlatformNormal) * vPlatformNormal;
		}

		m_DestinationPosX = vReflectionVec.x;
	}
}

