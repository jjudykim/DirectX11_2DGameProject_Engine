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

	// ī�޶��� ��ġ ��������
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

	// m_ReachMapLimit (0 : �� �� ���� X, 1 : ���� �� �� ����, 2 : ������ �� �� ����)
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
	
	float threshold = 100.0f;  // �÷��̾�� ī�޶��� X ��ǥ ���̰� �� ���� ������ ���� ����
	float correctionSpeed = 0.7f;  // ���� �ӵ��� ���� (���� �����Ͽ� ���� �ӵ� ����)
	
	if (fabs(distanceY) > threshold / 2.f)
	{
		// ���̿� ����� ���� �ӵ� ���
		float correctionVelocityY = distanceY * correctionSpeed;
	
		// ī�޶��� ���� �ӵ��� ���� �ӵ� �߰� (X �ุ ����)
		vPlayerGravityVelocity.y += correctionVelocityY;
	}

	m_Camera->RigidBody()->SetVelocityByGravity(vPlayerGravityVelocity * 0.5f);
}

void CReachLimitState::Exit()
{
}


