#include "pch.h"
#include "CTitleMenuScript.h"

#include "Engine\CLevelMgr.h"

CTitleMenuScript::CTitleMenuScript()
	: CScript(SCRIPT_TYPE::TITLEMENUSCRIPT)
{
}

CTitleMenuScript::~CTitleMenuScript()
{
}

void CTitleMenuScript::Begin()
{
	m_MaxMenuIndex = 2;
	m_CurMenuIndex = 0;
	m_IsLevelChanging = false;

	m_TextSelect = CLevelMgr::GetInst()->FindObjectByName(L"Text_Select");
	m_BtnSelectE = CLevelMgr::GetInst()->FindObjectByName(L"Btn_SelectE");
	m_Vehicle = CLevelMgr::GetInst()->FindObjectByName(L"Vehicle");
}

void CTitleMenuScript::Tick()
{
	if (KEY_TAP(KEY::DOWN))
	{
		m_CurMenuIndex++;

		if (m_CurMenuIndex > m_MaxMenuIndex)
			m_CurMenuIndex = 0;
	}

	if (KEY_TAP(KEY::UP))
	{
		m_CurMenuIndex--;

		if (m_CurMenuIndex < 0)
			m_CurMenuIndex = m_MaxMenuIndex;
	}

	if (KEY_TAP(KEY::E))
	{
		if (m_CurMenuIndex == 0)
		{
			m_IsLevelChanging = true;
			int FBIdx = m_Vehicle->FlipBookComponent()->FindFlipBookIndex(L"animation\\Main_Vehicle_Run.flip");
			m_Vehicle->FlipBookComponent()->Play(FBIdx, false);
		}
	}

	if (m_IsLevelChanging)
	{
		if (m_Vehicle->FlipBookComponent()->GetFinish())
		{
			CLevelMgr::GetInst()->RegisterChangeLevel(false, 0.f, 3);
			m_IsLevelChanging = false;
		}
			
	}

	if (m_CurMenuIndex == 0)
	{
		Vec3 highlightPos = m_TextSelect->Transform()->GetRelativePos();
		Vec3 btnPos = m_BtnSelectE->Transform()->GetRelativePos();
		m_TextSelect->Transform()->SetRelativePos(Vec3(highlightPos.x, -110.f, highlightPos.z));
		m_BtnSelectE->Transform()->SetRelativePos(Vec3(btnPos.x, -110.f, btnPos.z));
	}
	else if (m_CurMenuIndex == 1)
	{
		Vec3 highlightPos = m_TextSelect->Transform()->GetRelativePos();
		Vec3 btnPos = m_BtnSelectE->Transform()->GetRelativePos();
		m_TextSelect->Transform()->SetRelativePos(Vec3(highlightPos.x, -190.f, highlightPos.z));
		m_BtnSelectE->Transform()->SetRelativePos(Vec3(btnPos.x, -190.f, btnPos.z));
	}
	else if (m_CurMenuIndex == 2)
	{
		Vec3 highlightPos = m_TextSelect->Transform()->GetRelativePos();
		Vec3 btnPos = m_BtnSelectE->Transform()->GetRelativePos();
		m_TextSelect->Transform()->SetRelativePos(Vec3(highlightPos.x, -270.f, highlightPos.z));
		m_BtnSelectE->Transform()->SetRelativePos(Vec3(btnPos.x, -270.f, btnPos.z));
	}
}

void CTitleMenuScript::SaveToFile(FILE* _File)
{
}

void CTitleMenuScript::LoadFromFile(FILE* _File)
{
}


