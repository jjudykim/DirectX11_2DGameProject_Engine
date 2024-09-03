#include "pch.h"
#include "CRigidBody.h"

#define EARTH_GRAVITY_ACCEL 9.8f

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(0.f)
	, m_UseGravity(false)
	, m_GravityAccel(980.f)
	, m_MaxGravitySpeed(500.f)
	, m_Ground(false)
	, m_JumpSpeed(50.f)
	, m_GroundFunc(nullptr)
	, m_AirFunc(nullptr)
	, m_GroundInst(nullptr)
	, m_GroundDelegate(nullptr)
	, m_AirInst(nullptr)
	, m_AirDelegate(nullptr)
{
}

CRigidBody::CRigidBody(const CRigidBody& _Other)
	: CComponent(_Other)
	, m_GravityAccel(_Other.m_GravityAccel)
	, m_MaxGravitySpeed(_Other.m_MaxGravitySpeed)
	, m_UseGravity(_Other.m_UseGravity)			    
	, m_Ground(_Other.m_Ground)					    
	, m_JumpSpeed(_Other.m_JumpSpeed)			    
	, m_GroundFunc(nullptr)						    
	, m_AirFunc(nullptr)						    
	, m_GroundInst(nullptr)						    
	, m_GroundDelegate(nullptr)					    
	, m_AirInst(nullptr)						    
	, m_AirDelegate(nullptr)					    
{
}


CRigidBody::~CRigidBody()
{
}

void CRigidBody::Jump()
{
	m_VelocityByGravity += Vec3(0.f, 1.f, 0.f) * m_JumpSpeed;
	SetGround(false);
}

void CRigidBody::FinalTick()
{
	Vec3 vObjectPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vAccel = m_Force / m_Mass;

	// 중력 가속도에 의한 속도 증가
	if (m_UseGravity && !m_Ground)
	{
		m_VelocityByGravity += Vec3(0.f, -1.f, 0.f) * m_GravityAccel * DT;

		if (m_MaxGravitySpeed < m_VelocityByGravity.Length())
		{
			m_VelocityByGravity.Normalize();
			m_VelocityByGravity *= m_MaxGravitySpeed;
		}
	}

	// 현재 속도에 따른 이동
	// 속도 = 거리 / 시간	
	vObjectPos += m_VelocityByGravity * DT;
	GetOwner()->Transform()->SetRelativePos(vObjectPos);

	// DebugRender
	//DrawDebugRect(vObjectPos + vFinalVelocity, GetOwner()->Transform()->GetRelativeScale(), GetOwner()->Transform()->GetRelativeRotation(), Vec4(0.5f, 0.5f, 1.f, 1.f), 0.f, false);
}

void CRigidBody::SaveToFile(FILE* _File)
{
	//fwrite(&m_VelocityByGravity, sizeof(Vec3), 1, _File);

	fwrite(&m_GravityAccel, sizeof(float), 1, _File);
	fwrite(&m_MaxGravitySpeed, sizeof(float), 1, _File);

	fwrite(&m_UseGravity, sizeof(bool), 1, _File);
	fwrite(&m_Ground, sizeof(bool), 1, _File);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
}

void CRigidBody::LoadFromFile(FILE* _File)
{
	//fread(&m_VelocityByGravity, sizeof(Vec3), 1, _File);
	 
	fread(&m_GravityAccel, sizeof(float), 1, _File);
	fread(&m_MaxGravitySpeed, sizeof(float), 1, _File);
	
	fread(&m_UseGravity, sizeof(bool), 1, _File);
	fread(&m_Ground, sizeof(bool), 1, _File);
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
}
