#include "pch.h"
#include "CBossModeState.h"

CBossModeState::CBossModeState()
{
}

CBossModeState::~CBossModeState()
{
}


void CBossModeState::Set()
{
	CCameraState::Set();
}

void CBossModeState::Enter()
{
	m_Camera->Camera()->SetProjScale(1.6f);
}

void CBossModeState::FinalTick()
{
	Vec3 vPlayerVelocity = GetBlackboardData<Vec3>(L"PlayerVelocity");
	Vec3 vPlayerGravityVelocity = GetBlackboardData<Vec3>(L"PlayerGravityVelocity");
	Vec3 vPlayerPos = GetBlackboardData<Vec3>(L"PlayerPos");
	Vec3 vCameraPos = m_Camera->Transform()->GetRelativePos();
	
	m_ReachMapLimit = GetBlackboardData<int>(L"ReachLimit");
		
	if (m_ReachMapLimit != 0)
	{
		// m_ReachMapLimit (0 : 맵 끝 도달 X, 1 : 왼쪽 맵 끝 도달, 2 : 오른쪽 맵 끝 도달)
		if (m_ReachMapLimit == 2)
		{
			m_Camera->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			if (m_Camera->Transform()->GetRelativePos().x - 200.f > vPlayerPos.x)
				m_ReachMapLimit = 0;
				
		}
		else if (m_ReachMapLimit == 1)
		{
			m_Camera->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
			if (m_Camera->Transform()->GetRelativePos().x + 200.f < vPlayerPos.x)
				m_ReachMapLimit = 0;
		}
	}
	
	if (m_ReachMapLimit == 0)
	{
		Vec3 vCameraPos = m_Camera->Transform()->GetRelativePos();

		float distanceX = vPlayerPos.x - vCameraPos.x;

		float threshold = 150.0f;      // 플레이어와 카메라의 X 좌표 차이가 이 값을 넘으면 보정 적용
		float correctionSpeed = 0.5f;  // 보정 속도의 비율 (값을 조절하여 보정 속도 설정)

		if (fabs(distanceX) > threshold)
		{
			// 차이에 비례한 보정 속도 계산
			float correctionVelocityX = distanceX * correctionSpeed;

			// 카메라의 기존 속도에 보정 속도 추가 (X 축만 보정)
			vPlayerVelocity.x += correctionVelocityX;
		}

		m_Camera->RigidBody()->SetVelocity(vPlayerVelocity);
	}
}

void CBossModeState::Exit()
{
}

