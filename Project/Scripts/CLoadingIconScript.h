#pragma once
#include "Engine\CScript.h"

class CLoadingIconScript :
    public CScript
{
    CGameObject*         m_LoadingIcon;

    float                m_Time;
    bool                 m_IsTurning;
    bool                 m_IsReserved;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    void Turn(float _TotalTime);

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CLoadingIconScript);
    CLoadingIconScript();
    ~CLoadingIconScript();
};

