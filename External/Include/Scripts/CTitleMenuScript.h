#pragma once
#include "Engine\CScript.h"

class CTitleMenuScript :
    public CScript
{
    CGameObject*    m_TextSelect;
    CGameObject*    m_BtnSelectE;
    CGameObject*    m_Vehicle;

    int             m_MaxMenuIndex;
    int             m_CurMenuIndex;

    bool            m_IsLevelChanging;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CTitleMenuScript);
    CTitleMenuScript();
    ~CTitleMenuScript();
};

