#include "pch.h"
#include "CRenderComponent.h"

#include "CAssetMgr.h"
#include "CLevel.h"
#include "CLevelMgr.h"

CRenderComponent::CRenderComponent(COMPONENT_TYPE _Type)
	: CComponent(_Type)
	, m_Mesh(nullptr)
	, m_Mtrl(nullptr)
{
}

CRenderComponent::CRenderComponent(const CRenderComponent& _Origin)
	: CComponent(_Origin)
	, m_Mesh(_Origin.m_Mesh)
	, m_Mtrl(_Origin.m_Mtrl)
	, m_SharedMtrl(_Origin.m_Mtrl)
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel != nullptr)
	{
		assert(!(pCurLevel->GetState() != LEVEL_STATE::PLAY && nullptr != _Origin.m_DynamicMtrl));
	}

	if (nullptr != _Origin.m_DynamicMtrl)
	{
		GetDynamicMaterial();
	}
}

CRenderComponent::~CRenderComponent()
{
}

void CRenderComponent::SetMaterial(Ptr<CMaterial> _Mtrl)
{
	m_Mtrl = m_SharedMtrl = _Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetSharedMaterial()
{
	m_Mtrl = m_SharedMtrl;
	return m_Mtrl;
}

Ptr<CMaterial> CRenderComponent::GetDynamicMaterial()
{
	// Dynamic Mtrl의 생성 및 반환은 게임(Level)이 Play일 경우에만 가능
	assert(CLevelMgr::GetInst()->GetCurrentLevel()->GetState() == LEVEL_STATE::PLAY);

	if (m_DynamicMtrl != nullptr)
		return m_Mtrl = m_DynamicMtrl;

	m_Mtrl = m_DynamicMtrl = m_SharedMtrl->Clone();

	return m_Mtrl;
}

void CRenderComponent::SaveDataToFile(FILE* _File)
{
	SaveAssetRef(m_Mesh, _File);
	SaveAssetRef(m_Mtrl, _File);
	SaveAssetRef(m_SharedMtrl, _File);
}

void CRenderComponent::LoadDataFromFile(FILE* _File)
{
	LoadAssetRef(m_Mesh, _File);
	LoadAssetRef(m_Mtrl, _File);
	LoadAssetRef(m_SharedMtrl, _File);
}