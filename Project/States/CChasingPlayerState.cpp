#include "pch.h"
#include "CChasingPlayerState.h"

#include "Engine/CLevelMgr.h"

CChasingPlayerState::CChasingPlayerState()
{
}

CChasingPlayerState::~CChasingPlayerState()
{
}

void CChasingPlayerState::Set()
{
	CCameraState::Set();
}

void CChasingPlayerState::Enter()
{
}

void CChasingPlayerState::FinalTick()
{
	if (KEY_PRESSED(KEY::_1))
	{
		m_Owner->ChangeState(L"DirectControl");
	}

	if (GetBlackboardData<int>(L"ReachLimit") != 0)
	{
		m_Owner->ChangeState(L"ReachLimit");
	}

	Vec3 vPlayerVelocity = GetBlackboardData<Vec3>(L"PlayerVelocity");
	Vec3 vPlayerPos = GetBlackboardData<Vec3>(L"PlayerPos");

	Vec3 vCameraPos = m_Camera->Transform()->GetRelativePos();

	float distanceX = vPlayerPos.x - vCameraPos.x;

	float threshold = 100.0f;  // 플레이어와 카메라의 X 좌표 차이가 이 값을 넘으면 보정 적용
	float correctionSpeed = 0.7f;  // 보정 속도의 비율 (값을 조절하여 보정 속도 설정)

	if (fabs(distanceX) > threshold)
	{
		// 차이에 비례한 보정 속도 계산
		float correctionVelocityX = distanceX * correctionSpeed;

		// 카메라의 기존 속도에 보정 속도 추가 (X 축만 보정)
		vPlayerVelocity.x += correctionVelocityX;
	}

	m_Camera->RigidBody()->SetVelocity(vPlayerVelocity);
}

void CChasingPlayerState::Exit()
{
}


