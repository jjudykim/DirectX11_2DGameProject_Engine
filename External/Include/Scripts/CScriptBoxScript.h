#pragma once
#include "Engine\CScript.h"

class CScriptBoxScript :
    public CScript
{
private:
    wchar_t         m_FirstLineScript[256];
    wchar_t         m_SecondLineScript[256];
    wchar_t         m_ThirdLineScript[256];
    wchar_t         m_NPCName[256];

    int             m_ScriptContextIdx;

    CGameObject*    m_MainCamera;

    CGameObject*    m_BGPostProcess;
    CGameObject*    m_ScriptBox;
    CGameObject*    m_NPC;
    Vec3            m_NPCPos;

    CGameObject*    m_BtnE;

    bool            m_IsScriptActive;
    bool            m_IsAbleCloseScript;

public:
    void ActiveScriptBox(int ScriptContextIdx);
    bool IsScriptActive() { return m_IsScriptActive; }

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CScriptBoxScript);
    CScriptBoxScript();
    ~CScriptBoxScript();
};

