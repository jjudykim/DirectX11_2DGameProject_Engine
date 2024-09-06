#include "pch.h"
#include "CReachLimitState.h"

CReachLimitState::CReachLimitState()
{
}

CReachLimitState::~CReachLimitState()
{
}

void CReachLimitState::Set()
{
	CCameraState::Set();

	m_springConstant = 1.2f;
	m_dampingFactor = 0.5f;
	m_Velocity = Vec3(0.f, 0.f, 0.f);
}

void CReachLimitState::Enter()
{
	m_ReachMapLimit = GetBlackboardData<int>(L"ReachLimit");
	m_LimitTargetPos = GetBlackboardData<Vec3>(L"LimitTargetPos");
	m_LimitTargetScale = GetBlackboardData<Vec3>(L"LimitTargetScale");
	
	m_Velocity = m_Camera->RigidBody()->GetVelocity();
	float VelocityMagnitude = m_Velocity.Length();
	Vec3 vUnit = m_Velocity.Normalize() * -1.f;

	bool isOverLimit = false;

	// 카메라의 위치 가져오기
	Vec3 cameraPos = m_Camera->Transform()->GetRelativePos();
	Vec3 cameraScale = m_Camera->Transform()->GetWorldScale();

	if (m_ReachMapLimit == 2)
	{
		float StandardPosX = cameraPos.x + (cameraScale.x / 2.f);
		if (StandardPosX > m_LimitTargetPos.x - (m_LimitTargetScale.x / 2))
			isOverLimit = true;
	}
	else if (m_ReachMapLimit == 1)
	{
		float StandardPosX = cameraPos.x - (cameraScale.x / 2.f);
		if (StandardPosX < m_LimitTargetPos.x + (m_LimitTargetScale.x / 2))
			isOverLimit = true;
	}

	if (isOverLimit)
	{
		{
			m_Camera->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));

			Vec3 reboundForce = Vec3(-vUnit.x * 50.f, 0.f, 0.f);
			m_Camera->RigidBody()->AddForce(reboundForce);
		}
	}
	else
	{
		float Speed = 0.f;
		if (VelocityMagnitude <= 10.f)
		{
			Speed = VelocityMagnitude;
		}
		else
		{
			Speed = VelocityMagnitude * 100.f;
		}

		Vec3 springForce = vUnit * Speed * m_springConstant;
		Vec3 dampingForce = -m_Velocity * m_dampingFactor;

		Vec3 totalForce = springForce + dampingForce;

		m_Camera->RigidBody()->AddForce(totalForce);
	}
}

void CReachLimitState::FinalTick()
{
	Vec3 vPlayerPos = GetBlackboardData<Vec3>(L"PlayerPos");
	Vec3 vPlayerGravityVelocity = GetBlackboardData<Vec3>(L"PlayerGravityVelocity");
	Vec3 vCameraPos = m_Camera->Transform()->GetRelativePos();

	// m_ReachMapLimit (0 : 맵 끝 도달 X, 1 : 왼쪽 맵 끝 도달, 2 : 오른쪽 맵 끝 도달)
	if (m_ReachMapLimit == 2)
	{
		if (m_Camera->Transform()->GetRelativePos().x - 200.f > vPlayerPos.x)
			m_ReachMapLimit = 0;
	}
	else if (m_ReachMapLimit == 1)
	{
		if (m_Camera->Transform()->GetRelativePos().x + 200.f < vPlayerPos.x)
			m_ReachMapLimit = 0;
	}
	
	if (m_ReachMapLimit == 0)
	{
		m_Owner->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_ReachMapLimit);
		m_Owner->ChangeState(L"ChasingPlayer");
	}

	float distanceY = (vPlayerPos.y + 250.f) - vCameraPos.y;
	
	float threshold = 100.0f;  // 플레이어와 카메라의 X 좌표 차이가 이 값을 넘으면 보정 적용
	float correctionSpeed = 0.7f;  // 보정 속도의 비율 (값을 조절하여 보정 속도 설정)
	
	if (fabs(distanceY) > threshold / 2.f)
	{
		// 차이에 비례한 보정 속도 계산
		float correctionVelocityY = distanceY * correctionSpeed;
	
		// 카메라의 기존 속도에 보정 속도 추가 (X 축만 보정)
		vPlayerGravityVelocity.y += correctionVelocityY;
	}

	m_Camera->RigidBody()->SetVelocityByGravity(vPlayerGravityVelocity * 0.5f);
}

void CReachLimitState::Exit()
{
}


