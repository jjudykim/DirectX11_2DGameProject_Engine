#pragma once

enum class LEVEL_TYPE
{
    LOGO,
    LOADING,
    TITLE,
    LOBBY,
    LOBBYTOSTAGE,
    STAGE1,
    STAGE2,
    STAGETOBOSS,
    BOSS,

    END,
};

class CGameLevelMgr :
    public CSingleton<CGameLevelMgr>
{
    SINGLE(CGameLevelMgr);

private:
    CLevel*                     m_CurLevel;
    CLevel*                     m_NextLevel;
    CLevel*                     m_LoadingLevel;

    tLevelChangeInfo            m_RegistedLevelChangeInfo;
    map<LEVEL_TYPE, wstring>    LevelPathInfo;

public:
    void Init();
    void Tick();
    void RegisterChangeLevelWithLoad(float _Timer, LEVEL_TYPE _NextLevel);
    void ChangeLevelAfterLoad();
};

