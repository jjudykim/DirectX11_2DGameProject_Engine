#include "pch.h"
#include "CCameraMoveScript.h"

#include "Engine/CLevelMgr.h"
#include "States/CChasingPlayerState.h"
#include "States/CReachLimitState.h"

CCameraMoveScript::CCameraMoveScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAMOVESCRIPT))
	, m_CamSpeed(400.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "CameraSpeed", &m_CamSpeed);
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Begin()
{
	FSM()->SetBlackboardData(L"PlayerVelocity", DATA_TYPE::VEC3, &m_PlayerVelocity);
	FSM()->SetBlackboardData(L"PlayerGravityVelocity", DATA_TYPE::VEC3, &m_PlayerGravityVelocity);
	FSM()->SetBlackboardData(L"PlayerPos", DATA_TYPE::VEC3, &m_PlayerPos);
	FSM()->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	FSM()->SetBlackboardData(L"LimitTargetPos", DATA_TYPE::VEC3, &m_LimitTargetPos);
	FSM()->SetBlackboardData(L"LimitTargetScale", DATA_TYPE::VEC3, &m_LimitTargetScale);
	
	// FSM State
	FSM()->AddState(L"ChasingPlayer", new CChasingPlayerState);
	FSM()->AddState(L"ReachLimit", new CReachLimitState);

	FSM()->SetState();

	FSM()->ChangeState(L"ChasingPlayer");
}

void CCameraMoveScript::Tick()
{
	if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
	{
		OrthoGraphicMove();
	}
	else if (PROJ_TYPE::PERSPECTIVE == Camera()->GetProjType())
	{
		PerspectiveMove();
	}

	if (KEY_TAP(KEY::P))
	{
		if (PROJ_TYPE::ORTHOGRAPHIC == Camera()->GetProjType())
			Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);
		else
			Camera()->SetProjType(PROJ_TYPE::ORTHOGRAPHIC);
	}
}

void CCameraMoveScript::OrthoGraphicMove()
{	
	CGameObject* player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_PlayerVelocity = player->RigidBody()->GetVelocity();
	m_PlayerGravityVelocity = player->RigidBody()->GetVelocityByGravity();
	m_PlayerPos = player->Transform()->GetRelativePos();

	FSM()->SetBlackboardData(L"PlayerVelocity", DATA_TYPE::VEC3, &m_PlayerVelocity);
	FSM()->SetBlackboardData(L"PlayerGravityVelocity", DATA_TYPE::VEC3, &m_PlayerGravityVelocity);
	FSM()->SetBlackboardData(L"PlayerPos", DATA_TYPE::VEC3, &m_PlayerPos);
}

void CCameraMoveScript::PerspectiveMove()
{
	float Speed = m_CamSpeed;

	if (KEY_PRESSED(KEY::LSHIFT))
	{
		Speed *= 3.f;
	}

	Vec3 vFront = Transform()->GetWorldDir(DIR::FRONT);
	Vec3 vRight = Transform()->GetWorldDir(DIR::RIGHT);

	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::W))
	{
		vPos += vFront * DT * Speed;
	}
	
	if (KEY_PRESSED(KEY::S))
	{
		vPos -= vFront * DT * Speed;
	}
	
	if (KEY_PRESSED(KEY::A))
	{
		vPos -= vRight * DT * Speed;
	}
	
	if (KEY_PRESSED(KEY::D))
	{
		vPos += vRight * DT * Speed;
	}

	Transform()->SetRelativePos(vPos);

	if (KEY_PRESSED(KEY::RBTN))
	{
		CKeyMgr::GetInst()->MouseCapture(true);

		// ���콺�� �̵��ϴ� ����
		//vDir.x; ==> y�� ȸ��;
		//vDir.y; ==> x�� ȸ��
		Vec2 vDir = CKeyMgr::GetInst()->GetDragDir();

		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += vDir.x * XM_PI * DT * 10.f;
		vRot.x += vDir.y * XM_PI * DT * 10.f;
		Transform()->SetRelativeRotation(vRot);
	}
	else if (KEY_RELEASED(KEY::RBTN))
	{
		CKeyMgr::GetInst()->MouseCapture(false);
	}
}

void CCameraMoveScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	Vec3 vUnit = RigidBody()->GetVelocity().Normalize();
	if (vUnit.x < 0) m_Dir = UNITVEC_TYPE::LEFT;
	else if (vUnit.x > 0) m_Dir = UNITVEC_TYPE::RIGHT;

	if (m_Dir == UNITVEC_TYPE::LEFT)
		m_ReachMapLimit = 1;
	else if (m_Dir == UNITVEC_TYPE::RIGHT)
		m_ReachMapLimit = 2;

	// m_ReachMapLimit (0 : �� �� ���� X, 1 : ���� �� �� ����, 2 : ������ �� �� ����)
	FSM()->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	
	m_LimitTargetPos = _OtherCollider->GetWorldPos();
	m_LimitTargetScale = _OtherCollider->GetScale() * _OtherObject->Transform()->GetWorldScale();

	FSM()->SetBlackboardData(L"LimitTargetPos", DATA_TYPE::VEC3, &m_LimitTargetPos);
	FSM()->SetBlackboardData(L"LimitTargetScale", DATA_TYPE::VEC3, &m_LimitTargetScale);

}

void CCameraMoveScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_ReachMapLimit = 0;
	FSM()->SetBlackboardData(L"ReachLimit", DATA_TYPE::INT, &m_ReachMapLimit);
}

void CCameraMoveScript::SaveToFile(FILE* _File)
{
}

void CCameraMoveScript::LoadFromFile(FILE* _File)
{
}