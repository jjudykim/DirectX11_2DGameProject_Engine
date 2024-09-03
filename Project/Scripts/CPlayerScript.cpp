#include "pch.h"
#include "CPlayerScript.h"
#include "CMissileScript.h"

#include "Engine/CLevelMgr.h"
#include "Engine/CLevel.h"

// Using States
#include "States/CIdleState.h"
#include "States/CRunState.h"
#include "States/CJumpState.h"


CPlayerScript::CPlayerScript()
	: CScript(UINT(SCRIPT_TYPE::PLAYERSCRIPT))
	, m_Speed(400.f)
	, m_Dir(UNITVEC_TYPE::RIGHT)
{
	AddScriptParam(SCRIPT_PARAM::FLOAT, "PlayerSpeed", &m_Speed);
}

CPlayerScript::~CPlayerScript()
{
}

void CPlayerScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	FSM()->SetBlackboardData(L"Speed", DATA_TYPE::FLOAT, &m_Speed);
	FSM()->SetBlackboardData(L"Dir", DATA_TYPE::UNITVEC_TYPE, &m_Dir);
	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);

	// FSM State
	FSM()->AddState(L"Idle", new CIdleState);
	FSM()->AddState(L"Run", new CRunState);
	FSM()->AddState(L"Jump", new CJumpState);

	FSM()->SetState();

	FSM()->ChangeState(L"Idle");
}

void CPlayerScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();

	FSM()->SetBlackboardData(L"ReachMapLimit", DATA_TYPE::INT, &m_ReachMapLimit);
	// m_ReachMapLimit (0 : ¸Ê ³¡ µµ´Þ X, 1 : ¿ÞÂÊ ¸Ê ³¡ µµ´Þ, 2 : ¿À¸¥ÂÊ ¸Ê ³¡ µµ´Þ)

	if (m_OverlapPLTCount > 0)
	{
		RigidBody()->SetGround(true);
		RigidBody()->UseGravity(false);
	}
	else
	{
		RigidBody()->SetGround(false);
		RigidBody()->UseGravity(true);
	}

	if (KEY_TAP(KEY::SPACE))
	{
		FSM()->ChangeState(L"Jump");
	}

	Transform()->SetRelativePos(vPos);
}

void CPlayerScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{

	if (IsPlatformLayerObject(_OtherObject))
	{
		++m_OverlapPLTCount;
	}

	if (IsMapLimitObject(_OtherObject))
	{
		if (m_Dir == UNITVEC_TYPE::LEFT)
			m_ReachMapLimit = 1;
		else if (m_Dir == UNITVEC_TYPE::RIGHT)
			m_ReachMapLimit = 2;

	}
}

void CPlayerScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		float Correction = 0;

		Vec3 OtherColPos = _OtherCollider->GetWorldPos();
		Vec3 OtherColScale = _OtherCollider->GetScale() * _OtherObject->Transform()->GetWorldScale();
		Vec3 OwnColPos = _OwnCollider->GetWorldPos();
		Vec3 OwnColScale = _OwnCollider->GetScale() * GetOwner()->Transform()->GetWorldScale();

		if (Vec3(0, 0, 0) != _OtherObject->Transform()->GetRelativeRotation())
		{
			Vec3 OtherRot = _OtherObject->Transform()->GetRelativeRotation();
			Matrix matRot = XMMatrixRotationX(OtherRot.x) * XMMatrixRotationY(OtherRot.y) * XMMatrixRotationZ(OtherRot.z);
			OtherColPos = XMVector3TransformCoord(OtherColPos, matRot);
		}

		float Distance = fabs(OtherColPos.y - OwnColPos.y);
		float Standard = (OtherColScale.y / 2.f) + (OwnColScale.y / 2.f);

		if (Distance < Standard)
		{
			Correction = Standard - Distance;
		}

		Vec3 vPos = Transform()->GetRelativePos();
		vPos.y += Correction;
		Transform()->SetRelativePos(vPos);
	}
}


void CPlayerScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		--m_OverlapPLTCount;
	}

	if (IsMapLimitObject(_OtherObject))
	{
		m_ReachMapLimit = 0;
	}
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