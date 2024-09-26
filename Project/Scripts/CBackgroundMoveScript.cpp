#include "pch.h"
#include "CBackgroundMoveScript.h"

#include "Engine\CLevelMgr.h"

CBackgroundMoveScript::CBackgroundMoveScript()
	: CScript(SCRIPT_TYPE::BACKGROUNDMOVESCRIPT)
{
}

CBackgroundMoveScript::~CBackgroundMoveScript()
{
}

void CBackgroundMoveScript::Begin()
{
	m_BackgroundLayer = GetOwner()->GetLayerIdx();

	m_OriginPos = Transform()->GetRelativePos();
	m_MainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
}

void CBackgroundMoveScript::Tick()
{
	float fCameraFocusX = m_MainCamera->Transform()->GetRelativePos().x;

	if (m_BackgroundLayer == (int)LAYER_TYPE::BACKGROUND3)
	{
		Transform()->SetRelativePos(Vec3(m_OriginPos.x + (fCameraFocusX * 0.12f), m_OriginPos.y, m_OriginPos.z));
	}
	else if (m_BackgroundLayer == (int)LAYER_TYPE::BACKGROUND2)
	{
		Transform()->SetRelativePos(Vec3(m_OriginPos.x + (fCameraFocusX * 0.08f), m_OriginPos.y, m_OriginPos.z));
	}
	else if (m_BackgroundLayer == (int)LAYER_TYPE::BACKGROUND1)
	{
		Transform()->SetRelativePos(Vec3(m_OriginPos.x + (fCameraFocusX * 0.04f), m_OriginPos.y, m_OriginPos.z));
	}
}

void CBackgroundMoveScript::SaveToFile(FILE* _File)
{
}

void CBackgroundMoveScript::LoadFromFile(FILE* _File)
{
}


