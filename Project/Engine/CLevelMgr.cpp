#include "pch.h"
#include "CLevelMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"

#include "CTaskMgr.h"

CLevelMgr::CLevelMgr()
	: m_CurLevel(nullptr)
{
}

CLevelMgr::~CLevelMgr()
{
	if (m_CurLevel != nullptr)
		delete m_CurLevel;
}

CGameObject* CLevelMgr::FindObjectByName(const wstring& _Name)
{
	if (m_CurLevel)
	{
		return m_CurLevel->FindObjectByName(_Name);
	}

	return nullptr;
}

void CLevelMgr::LevelChanged()
{
	// Level에 변경점이 발생한 경우의 Task
	CTaskMgr::GetInst()->AddTask(tTask{ TASK_TYPE::LEVEL_CHANGED });
}

void CLevelMgr::Init()
{
	m_LevelChangeInfo = { false, 0.f, 9 };
}

void CLevelMgr::Progress()
{
	m_LevelChanged = false;

	if (m_CurLevel == nullptr)
		return;

	if (m_CurLevel->GetState() == LEVEL_STATE::PLAY)
	{
		m_CurLevel->Tick();
	}
	m_CurLevel->ClearObject();
	m_CurLevel->FinalTick();
}

bool CLevelMgr::ChangeLevel(CLevel* _NextLevel)
{
	if (m_CurLevel == _NextLevel)
		return false;

	if (m_CurLevel != nullptr)
		delete m_CurLevel;

	m_CurLevel = _NextLevel;

	// Level이 변경된 Event On
	LevelChanged();

	return true;
}

void CLevelMgr::RegisterChangeLevel(bool _WithLoading, float _Timer, UINT _LevelType)
{
	m_LevelChangeInfo = { _WithLoading, _Timer, _LevelType };
}