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
		return;
	}

	Vec3 vPlayerVelocity = GetBlackboardData<Vec3>(L"PlayerVelocity");
	Vec3 vPlayerGravityVelocity = GetBlackboardData<Vec3>(L"PlayerGravityVelocity");
	Vec3 vPlayerPos = GetBlackboardData<Vec3>(L"PlayerPos");

	Vec3 vCameraPos = m_Camera->Transform()->GetRelativePos();

	float distanceX = vPlayerPos.x - vCameraPos.x;
	float distanceY = (vPlayerPos.y + 250.f) - vCameraPos.y;

	float threshold = 100.0f;  // �÷��̾�� ī�޶��� X ��ǥ ���̰� �� ���� ������ ���� ����
	float correctionSpeed = 0.7f;  // ���� �ӵ��� ���� (���� �����Ͽ� ���� �ӵ� ����)

	if (fabs(distanceX) > threshold)
	{ 
		// ���̿� ����� ���� �ӵ� ���
		float correctionVelocityX = distanceX * correctionSpeed;

		// ī�޶��� ���� �ӵ��� ���� �ӵ� �߰� (X �ุ ����)
		vPlayerVelocity.x += correctionVelocityX;
	}

	if (fabs(distanceY) > threshold / 2.f)
	{
		// ���̿� ����� ���� �ӵ� ���
		float correctionVelocityY = distanceY * correctionSpeed;

		// ī�޶��� ���� �ӵ��� ���� �ӵ� �߰� (X �ุ ����)
		vPlayerGravityVelocity.y += correctionVelocityY;
	}

	m_Camera->RigidBody()->SetVelocity(vPlayerVelocity);
	m_Camera->RigidBody()->SetVelocityByGravity(vPlayerGravityVelocity * 0.7f);
}

void CChasingPlayerState::Exit()
{
}


