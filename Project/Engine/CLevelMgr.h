#pragma once
#include "singleton.h"

class CLevel;
class CGameObject;

class CLevelMgr :
    public CSingleton<CLevelMgr>
{
    SINGLE(CLevelMgr);

private:
    CLevel*                    m_CurLevel;
    tLevelChangeInfo           m_LevelChangeInfo;

    bool                       m_LevelChanged;

public:
    CLevel* GetCurrentLevel() { return m_CurLevel; }
    CGameObject* FindObjectByName(const wstring& _Name);

    void LevelChanged();
    bool IsLevelChanged() { return m_LevelChanged; }
    void RegisterChangeLevel(bool _WithLoading, float _Timer, UINT _LevelType);

    const tLevelChangeInfo& GetLevelChangeInfo() { return m_LevelChangeInfo; }
    void ClearLevelChangeInfo(UINT _LevelEnd) { m_LevelChangeInfo = { 0, 0.f, _LevelEnd }; }

private:
    bool ChangeLevel(CLevel* _NextLevel);

public:
public:
    void Init();
    void Progress();

    friend class CTaskMgr;
};