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

	float threshold = 100.0f;  // �÷��̾�� ī�޶��� X ��ǥ ���̰� �� ���� ������ ���� ����
	float correctionSpeed = 0.7f;  // ���� �ӵ��� ���� (���� �����Ͽ� ���� �ӵ� ����)

	if (fabs(distanceX) > threshold)
	{
		// ���̿� ����� ���� �ӵ� ���
		float correctionVelocityX = distanceX * correctionSpeed;

		// ī�޶��� ���� �ӵ��� ���� �ӵ� �߰� (X �ุ ����)
		vPlayerVelocity.x += correctionVelocityX;
	}

	m_Camera->RigidBody()->SetVelocity(vPlayerVelocity);
}

void CChasingPlayerState::Exit()
{
}


