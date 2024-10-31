#include "pch.h"
#include "CLoadingIconScript.h"

#include "Engine\CLevelMgr.h"

CLoadingIconScript::CLoadingIconScript()
	: CScript(SCRIPT_TYPE::LOADINGICONSCRIPT)
{
}

CLoadingIconScript::~CLoadingIconScript()
{
}

void CLoadingIconScript::Begin()
{
	m_LoadingIcon = CLevelMgr::GetInst()->FindObjectByName(L"LoadingIcon");

	m_IsReserved = false;
	assert(m_LoadingIcon);
}

void CLoadingIconScript::Tick()
{
	if (!m_IsTurning && !m_IsReserved)
	{
		CTimeMgr::GetInst()->AddTimer(3.0f, [this]() { m_IsTurning = true; }, false);
		m_IsReserved = true;
	}
	else
	{
		Turn(1.0f);
	}
}

void CLoadingIconScript::Turn(float _TotalTime = 1.0f)
{
	if (m_Time > _TotalTime)
	{
		m_IsTurning = false;
		m_LoadingIcon->Transform()->SetRelativeRotation(Vec3(0.f, 0.f, 0.f));
		m_Time = 0.f;
		m_IsReserved = false;
	}
	else
	{
		float maxAngularVelocity = XM_PI * 3.2f;
		float t = m_Time / _TotalTime;

		float angularVelocity = maxAngularVelocity * sinf(t * XM_PI); // 0에서 최대 속도까지 가속 후 감속
		float deltaRotation = angularVelocity * DT; // 매 Tick 회전량

		// Z축 기준 회전
		Vec3 vRot = m_LoadingIcon->Transform()->GetRelativeRotation();
		m_LoadingIcon->Transform()->SetRelativeRotation(vRot.x, vRot.y, vRot.z + deltaRotation);

		m_Time += DT;
	}
}

void CLoadingIconScript::SaveToFile(FILE* _File)
{
}

void CLoadingIconScript::LoadFromFile(FILE* _File)
{
}


