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
		// m_ReachMapLimit (0 : �� �� ���� X, 1 : ���� �� �� ����, 2 : ������ �� �� ����)
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

		float threshold = 150.0f;      // �÷��̾�� ī�޶��� X ��ǥ ���̰� �� ���� ������ ���� ����
		float correctionSpeed = 0.5f;  // ���� �ӵ��� ���� (���� �����Ͽ� ���� �ӵ� ����)

		if (fabs(distanceX) > threshold)
		{
			// ���̿� ����� ���� �ӵ� ���
			float correctionVelocityX = distanceX * correctionSpeed;

			// ī�޶��� ���� �ӵ��� ���� �ӵ� �߰� (X �ุ ����)
			vPlayerVelocity.x += correctionVelocityX;
		}

		m_Camera->RigidBody()->SetVelocity(vPlayerVelocity);
	}
}

void CBossModeState::Exit()
{
}

