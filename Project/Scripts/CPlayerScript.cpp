#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"

// Using States
#include "States/CIdleState.h"
#include "States/CRunState.h"


CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(600.f)
	, m_Dir(UNITVEC_TYPE::RIGHT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
	//AddScriptParam(SCRIPT_PARAM::TEXTURE, "Test", &m_Texture);
	//AddScriptParam(SCRIPT_PARAM::PREFAB, "Missile", &m_MissilePref);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	if (FSM() == nullptr)
		GetOwner()->AddComponent(new CFSM);

	//m_MissilePref = CAssetMgr::GetInst()->FindAsset<CPrefab>(L"MissilePref");
	FSM()->SetBlackboardData(L"Speed", DATA_TYPE::FLOAT, &m_Speed);
	FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
	
	// FSM State
	FSM()->AddState(L"Idle", new CIdleState);
	m_vStatesStr.push_back(L"Idle");
	FSM()->AddState(L"Run", new CRunState);
	m_vStatesStr.push_back(L"Run");

	FSM()->SetState();

	FSM()->ChangeState(L"Idle");
}

void CPlayerScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	if (KEY_PRESSED(KEY::LEFT))
	{
		m_Dir = UNITVEC_TYPE::LEFT;
		FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		Transform()->SetRelativeRotation(Vec3(XMConvertToRadians(180.f), 0.f, XMConvertToRadians(180.f)));
		FSM()->ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::RIGHT))
	{
		m_Dir = UNITVEC_TYPE::RIGHT;
		FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
		Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		FSM()->ChangeState(L"Run");
	}

	if (KEY_PRESSED(KEY::Q))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y += XM_PI * DT;
		Transform()->SetRelativeRotation(vRot);
	}

	if (KEY_PRESSED(KEY::E))
	{
		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.y -= XM_PI * DT;
		Transform()->SetRelativeRotation(vRot);
	}

	if (KEY_PRESSED(KEY::Z))
	{
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, 1);

		Vec3 vRot = Transform()->GetRelativeRotation();
		vRot.z += DT * XM_PI;
		Transform()->SetRelativeRotation(vRot);

		/*Vec3 vScale = Transform()->GetRelativeScale();
		vScale += DT * 100.f * Vec3(1.f, 1.f, 1.f);
		Transform()->SetRelativeScale(vScale);*/
	}
	else
	{
		MeshRender()->GetMaterial()->SetScalarParam(INT_0, 0);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		// 미사일 발사
		if (m_MissilePref != nullptr)
		{
			Instantiate(m_MissilePref, 5, Transform()->GetWorldPos(), L"Missile");
		}
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CPlayerScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, sizeof(float), 1, _File);
	SaveAssetRef(m_Texture, _File);
	SaveAssetRef(m_MissilePref, _File);
}

void CPlayerScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, sizeof(float), 1, _File);
	LoadAssetRef(m_Texture, _File);
	LoadAssetRef(m_MissilePref, _File);
}