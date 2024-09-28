#include "pch.h"
#include "CGameLevelMgr.h"

#include "CLevelSaveLoad.h"
#include <Engine\CLevelMgr.h>

CGameLevelMgr::CGameLevelMgr()
{

    wstring LevelPath = CPathMgr::GetInst()->GetContentPath() + L"level\\";
    
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::LOGO, LevelPath + L"Logo.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::LOADING, LevelPath + L"Loading.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::TITLE, LevelPath + L"Title.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::STAGE1, LevelPath + L"DarkChat1.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::STAGE2, LevelPath + L"DarkChat2.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::BOSS, LevelPath + L"DarkChat_Boss.lv"));
}

CGameLevelMgr::~CGameLevelMgr()
{
}

void CGameLevelMgr::Init()
{
    m_CurLevel = CLevelSaveLoad::LoadLevel(LevelPathInfo.find(LEVEL_TYPE::LOGO)->second);
    m_LoadingLevel = CLevelSaveLoad::LoadLevel(LevelPathInfo.find(LEVEL_TYPE::LOADING)->second);
    
    CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)m_CurLevel, LEVEL_STATE::PLAY });

    CTimeMgr::GetInst()->AddTimer(3.0f, [this]() { RegisterChangeLevelWithLoad(LEVEL_TYPE::TITLE); }, false);
}

void CGameLevelMgr::Tick()
{
}

void CGameLevelMgr::RegisterChangeLevelWithLoad(LEVEL_TYPE _NextLevel)
{
    m_CurLevel = m_LoadingLevel;
    CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)m_CurLevel, LEVEL_STATE::PLAY });

    m_NextLevel = CLevelSaveLoad::LoadLevel(LevelPathInfo.find(_NextLevel)->second);

    CTimeMgr::GetInst()->AddTimer(3.0f, [this]() { ChangeLevelAfterLoad(); }, false);
}

void CGameLevelMgr::ChangeLevelAfterLoad()
{
    CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)m_NextLevel, LEVEL_STATE::PLAY });

    m_CurLevel = m_NextLevel;
}

