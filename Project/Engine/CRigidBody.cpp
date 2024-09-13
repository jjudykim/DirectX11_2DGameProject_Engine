#include "pch.h"
#include "CRigidBody.h"

#define EARTH_GRAVITY_ACCEL 9.8f

CRigidBody::CRigidBody()
	: CComponent(COMPONENT_TYPE::RIGIDBODY)
	, m_Mass(1.f)
	, m_InitWalkSpeed(0.f)
	, m_MaxWalkSpeed(0.f)
	, m_Friction(500.f)
	, m_GravityAccel(980.f)
	, m_MaxGravitySpeed(0.f)
	, m_JumpSpeed(400.f)
	, m_UseGravity(false)
	, m_Ground(false)
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
	, m_Mass(_Other.m_Mass)
	, m_InitWalkSpeed(_Other.m_InitWalkSpeed)
	, m_MaxWalkSpeed(_Other.m_MaxWalkSpeed)
	, m_MaxGravitySpeed(_Other.m_MaxGravitySpeed)
	, m_Friction(_Other.m_Friction)
	, m_GravityAccel(_Other.m_GravityAccel)
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
}

void CRigidBody::CurvedJump(bool _Replay, UNITVEC_TYPE _Dir, float _Time)
{
	static float angle = 0.0f;          // 각도가 시간에 따라 변화

	if (_Replay)
		angle = 0.0f;

	float radius = 1.0f;                
	float angularSpeed = 3.7f;

	angle += angularSpeed * _Time;

	if (_Dir == UNITVEC_TYPE::RIGHT)
		m_Velocity.x = cos(angle) * radius * m_JumpSpeed * 0.075f;
	else
		m_Velocity.x = cos(angle) * radius * m_JumpSpeed * 0.075f * -1.f;

	m_VelocityByGravity.y = sin(angle) * radius * m_JumpSpeed * 0.15f;

	// 점프가 완료되었을 때 각도 초기화
	//if (angle >= 125.0f * PI / 180.0f) // 90도 도달 시 점프 종료 (원하는 높이에 도달했을 때)
	//{
	//	angle = 0.0f; // 각도 초기화
	//}
}
void CRigidBody::FinalTick()
{
	Vec3 vObjectPos = GetOwner()->Transform()->GetRelativePos();
	Vec3 vAccel = m_Force / m_Mass;

	// 가속도
	if (m_UseGravity && !m_Ground)           // 중력을 사용할때, 공중에서 힘이 적용된 경우
	{
		m_Velocity += vAccel * DT * 0.3f;
	}
	else
	{
		m_Velocity += vAccel * DT;
	}

	// 최대 속도 제한
	if (0.f != m_MaxWalkSpeed && m_MaxWalkSpeed < m_Velocity.Length())
	{
		m_Velocity.Normalize();
		m_Velocity *= m_MaxWalkSpeed;
	}

	// 최소 속도 보정
	if (m_InitWalkSpeed != 0.f && m_Velocity.Length() < 10.f && vAccel != Vec3(0.f, 0.f, 0.f))
	{
		Vec3 vForceDir = m_Force;
		vForceDir.Normalize();
		m_Velocity = vForceDir * m_InitWalkSpeed;
	}

	// 힘은 없지만, 속도는 있는 경우 마찰력 적용
	if(m_Force == Vec3(0.f, 0.f, 0.f))
	{		
		float Speed = m_Velocity.Length();

		Speed -= m_Friction * DT;
		
		if (Speed < 0)
			Speed = 0.f;

		if (m_Velocity != Vec3(0.f, 0.f, 0.f))
			m_Velocity.Normalize();

		m_Velocity *= Speed;
	}

	// 중력 가속도에 의한 속도 증가
	if (m_UseGravity && !m_Ground)
	{
		m_VelocityByGravity += Vec3(0.f, -1.f, 0.f) * m_GravityAccel * DT;

		if (m_VelocityByGravity.y < 0.f && m_MaxGravitySpeed != 0.f && m_MaxGravitySpeed < m_VelocityByGravity.Length())
		{
			m_VelocityByGravity.Normalize();
			m_VelocityByGravity *= m_MaxGravitySpeed;
		}
	}

	// 최종 속도
	Vec3 vFinalVelocity = m_Velocity + m_VelocityByGravity;

	// 현재 속도에 따른 이동
	// 속도 = 거리 / 시간	
	vObjectPos += vFinalVelocity * DT;
	GetOwner()->Transform()->SetRelativePos(vObjectPos);

	// 이번 프레임 힘 초기화
	m_Force = Vec3(0.f, 0.f, 0.f);
	m_AddVelocity = Vec3(0.f, 0.f, 0.f);

	// DebugRender
	//DrawDebugRect(vObjectPos + vFinalVelocity, GetOwner()->Transform()->GetRelativeScale(), GetOwner()->Transform()->GetRelativeRotation(), Vec4(0.5f, 0.5f, 1.f, 1.f), 0.f, false);
}

void CRigidBody::SaveToFile(FILE* _File)
{
	fwrite(&m_Mass, sizeof(float), 1, _File);
	fwrite(&m_InitWalkSpeed, sizeof(float), 1, _File);
	fwrite(&m_MaxWalkSpeed, sizeof(float), 1, _File);
	fwrite(&m_Friction, sizeof(float), 1, _File);
	fwrite(&m_GravityAccel, sizeof(float), 1, _File);
	fwrite(&m_MaxGravitySpeed, sizeof(float), 1, _File);
	fwrite(&m_JumpSpeed, sizeof(float), 1, _File);
	fwrite(&m_UseGravity, sizeof(bool), 1, _File);
}

void CRigidBody::LoadFromFile(FILE* _File)
{
	fread(&m_Mass, sizeof(float), 1, _File);
	fread(&m_InitWalkSpeed, sizeof(float), 1, _File);
	fread(&m_MaxWalkSpeed, sizeof(float), 1, _File);
	fread(&m_Friction, sizeof(float), 1, _File);
	fread(&m_GravityAccel, sizeof(float), 1, _File);
	fread(&m_MaxGravitySpeed, sizeof(float), 1, _File);
	fread(&m_JumpSpeed, sizeof(float), 1, _File);
	fread(&m_UseGravity, sizeof(bool), 1, _File);
}
