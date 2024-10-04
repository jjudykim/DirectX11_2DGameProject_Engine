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
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::LOBBY, LevelPath + L"Lobby.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::LOBBYTOSTAGE, LevelPath + L"LobbyToStage.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::STAGE1, LevelPath + L"DarkChat1.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::STAGE2, LevelPath + L"DarkChat2.lv"));
    LevelPathInfo.insert(make_pair(LEVEL_TYPE::STAGETOBOSS, LevelPath + L"StageToBoss.lv"));
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

    CTimeMgr::GetInst()->AddTimer(3.0f, [this]() { RegisterChangeLevelWithLoad(3.0f, LEVEL_TYPE::TITLE); }, false);
}

void CGameLevelMgr::Tick()
{
    m_RegistedLevelChangeInfo = CLevelMgr::GetInst()->GetLevelChangeInfo();
    if (m_RegistedLevelChangeInfo.LevelType != (UINT)LEVEL_TYPE::END)
    {
        if (m_RegistedLevelChangeInfo.WithLoading == true)
        {
            RegisterChangeLevelWithLoad(m_RegistedLevelChangeInfo.Timer, (LEVEL_TYPE)m_RegistedLevelChangeInfo.LevelType);
        }
        else
        {
            m_NextLevel = CLevelSaveLoad::LoadLevel(LevelPathInfo.find((LEVEL_TYPE)m_RegistedLevelChangeInfo.LevelType)->second);
            ChangeLevelAfterLoad();
        }
    }
}

void CGameLevelMgr::RegisterChangeLevelWithLoad(float _Timer, LEVEL_TYPE _NextLevel)
{
    m_CurLevel = m_LoadingLevel;
    CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)m_CurLevel, LEVEL_STATE::PLAY });

    m_NextLevel = CLevelSaveLoad::LoadLevel(LevelPathInfo.find(_NextLevel)->second);

    CTimeMgr::GetInst()->AddTimer(_Timer, [this]() { ChangeLevelAfterLoad(); }, false);
}

void CGameLevelMgr::ChangeLevelAfterLoad()
{
    CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CHANGE_LEVEL, (DWORD_PTR)m_NextLevel, LEVEL_STATE::PLAY });

    m_CurLevel = m_NextLevel;

    CLevelMgr::GetInst()->ClearLevelChangeInfo((UINT)LEVEL_TYPE::END);
}

