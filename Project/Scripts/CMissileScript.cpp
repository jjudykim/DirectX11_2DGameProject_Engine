#include "pch.h"
#include "CMissileScript.h"

CMissileScript::CMissileScript()
	: CScript(UINT(SCRIPT_TYPE::MISSILESCRIPT))
	, m_Speed(1000.f)
{
}

CMissileScript::~CMissileScript()
{
}

void CMissileScript::Begin()
{
	GetRenderComponent()->GetDynamicMaterial();

	Ptr<CMaterial> pMtrl = GetRenderComponent()->GetMaterial();

	if (pMtrl != nullptr)
	{
		Ptr<CTexture> pTexture = CAssetMgr::GetInst()->Load<CTexture>(L"texture\\cuchilla.png", L"texture\\cuchilla.png");
		pMtrl->SetTexParam(TEX_0, pTexture);
	}
}

void CMissileScript::Tick()
{
	Vec3 vPos = Transform()->GetRelativePos();
	
	vPos.y += DT * m_Speed;

	Transform()->SetRelativePos(vPos);
}

void CMissileScript::BeginOverlap(CCollider2D* _OwnCollier, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	DeleteObject(_OtherObject);
}

void CMissileScript::SaveToFile(FILE* _File)
{
	fwrite(&m_Speed, 4, 1, _File);
}

void CMissileScript::LoadFromFile(FILE* _File)
{
	fread(&m_Speed, 4, 1, _File);
}
