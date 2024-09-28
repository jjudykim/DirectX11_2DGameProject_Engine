#pragma once

enum LEVEL_TYPE
{
    LOGO,
    LOADING,
    TITLE,
    STAGE1,
    STAGE2,
    BOSS,
};

class CGameLevelMgr :
    public CSingleton<CGameLevelMgr>
{
    SINGLE(CGameLevelMgr);

private:
    CLevel*                     m_CurLevel;
    CLevel*                     m_NextLevel;
    CLevel*                     m_LoadingLevel;

    map<LEVEL_TYPE, wstring>    LevelPathInfo;

public:
    void Init();
    void Tick();
    void RegisterChangeLevelWithLoad(LEVEL_TYPE _NextLevel);
    void ChangeLevelAfterLoad();
};

