#include "pch.h"
#include "CCameraMoveScript.h"

#include "Engine/CLevelMgr.h"
#include "States/CChasingPlayerState.h"
#include "States/CReachLimitState.h"

CCameraMoveScript::CCameraMoveScript()
	: CScript(UINT(SCRIPT_TYPE::CAMERAMOVESCRIPT))
	, m_CamSpeed(300.f)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "CameraSpeed", &m_CamSpeed);
}

CCameraMoveScript::~CCameraMoveScript()
{
}

void CCameraMoveScript::Begin()
{
	FSM()->SetBlackboardData(L"CamSpeed", DATA_TYPE::FLOAT, &m_CamSpeed);
	FSM()->SetBlackboardData(L"CamDir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
	FSM()->SetBlackboardData(L"StandardPos", DATA_TYPE::VEC3, &m_StandardPos);
	
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
	//float Speed = m_CamSpeed;
	//
	////Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
	//Vec3 vPos = Transform()->GetRelativePos();
	//
	CGameObject* player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_CamSpeed = player->FSM()->GetBlackboardData<float>(L"Speed");
	FSM()->SetBlackboardData(L"CamSpeed", DATA_TYPE::FLOAT, &m_CamSpeed);

	m_StandardPos = player->Transform()->GetRelativePos();
	m_StandardPos.y += 300;
	FSM()->SetBlackboardData(L"StandardPos", DATA_TYPE::VEC3, m_StandardPos);
	//
	//if (m_FollowPlayer)
	//{
	//	if (m_IsReachLimit)
	//	{
	//		if (fabs(m_LimitPos.x - Transform()->GetRelativePos().x) > 5.f
	//			|| fabs(m_LimitPos.y - Transform()->GetRelativePos().y) > 5.f)
	//		{
	//			vPos += m_CurDir * DT * m_CamSpeed;
	//		}
	//		else
	//		{
	//			if (fabs(vPos.x - vStandardPos.x) > 250 || fabs(vPos.y - vStandardPos.y) > 250)
	//			{
	//				m_IsReachLimit = false;
	//			}
	//		}
	//	}
	//	else
	//	{
	//		if (fabs(vPos.x - vStandardPos.x) > 170 || fabs(vPos.y - vStandardPos.y) > 100)
	//		{
	//			Vec3 vUnit = vStandardPos - vPos;
	//			float length = vUnit.Length();
	//			m_CurDir = vUnit / length;
	//
	//			vPos += m_CurDir * DT * m_CamSpeed;
	//		}
	//	}
	//	
	//}
	//else
	//{
	//	if (KEY_PRESSED(KEY::W))
	//	{
	//		vPos.y += DT * Speed;
	//	}
	//	
	//	if (KEY_PRESSED(KEY::S))
	//	{
	//		vPos.y -= DT * Speed;
	//	}
	//	
	//	if (KEY_PRESSED(KEY::A))
	//	{
	//		vPos.x -= DT * Speed;
	//	}
	//	
	//	if (KEY_PRESSED(KEY::D))
	//	{
	//		vPos.x += DT * Speed;
	//	}
	//}
	//
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

		// 마우스가 이동하는 방향
		//vDir.x; ==> y축 회전;
		//vDir.y; ==> x축 회전
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
	//m_IsReachLimit = true;
	//m_LimitPos = Transform()->GetRelativePos();
	//if (m_CurDir.x == 1) m_LimitPos.x -= 100;
	//if (m_CurDir.x == -1) m_LimitPos.x += 100;
	//if (m_CurDir.y == 1) m_LimitPos.y -= 100;
	//if (m_CurDir.y == -1) m_LimitPos.y += 100;
	//
	//m_CurDir *= -1;
}

void CCameraMoveScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	//m_IsReachLimit = false;
}

void CCameraMoveScript::SaveToFile(FILE* _File)
{
	fwrite(&m_CamSpeed, sizeof(float), 1, _File);
}

void CCameraMoveScript::LoadFromFile(FILE* _File)
{
	fread(&m_CamSpeed, sizeof(float), 1, _File);
}