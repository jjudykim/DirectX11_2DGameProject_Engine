#include "pch.h"
#include "CArcProjectileScript.h"

#include "Engine/CLevelMgr.h"

CArcProjectileScript::CArcProjectileScript()
	: CScript((UINT)SCRIPT_TYPE::ARCPROJECTILESCRIPT)
{

}

CArcProjectileScript::~CArcProjectileScript()
{
	
}

void CArcProjectileScript::Begin()
{
	m_Player = CLevelMgr::GetInst()->FindObjectByName(L"Player");
	m_IsExploding = false;

	Vec3 vPlayerPos = m_Player->Transform()->GetRelativePos();
	m_DestinationPosX = Transform()->GetRelativePos().x - vPlayerPos.x;

	RigidBody()->AddVelocityByGravity(Vec3(0.f, 500.f, 0.f));
}

void CArcProjectileScript::Tick()
{
	if (!m_IsExploding)
	{
		Vec3 vCurRelativeScale = Transform()->GetRelativeScale();
		Transform()->SetRelativeScale(vCurRelativeScale * 1.0015f);

		m_VelocityX = m_DestinationPosX * (-2.f);
		RigidBody()->AddForce(Vec3(m_VelocityX, 0.f, 0.f));
	}
	else
	{
		CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)GetOwner()});
	}
}

void CArcProjectileScript::SaveToFile(FILE* _File)
{
}

void CArcProjectileScript::LoadFromFile(FILE* _File)
{
}

void CArcProjectileScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	if (IsPlatformLayerObject(_OtherObject))
	{
		RigidBody()->SetVelocityByGravity(Vec3(0.f, 0.f, 0.f));
		RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
		m_IsExploding = true;
	}
}