#pragma once

#include "Engine/CState.h"
#include "Engine/CGameObject.h"

class CCameraState :
    public CState
{
protected:
    float                m_CamSpeed;
    UNITVEC_TYPE         m_Dir;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CCameraState();
    ~CCameraState();

};

