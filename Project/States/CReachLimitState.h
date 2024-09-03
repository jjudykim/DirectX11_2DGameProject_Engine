#pragma once
#include "CCameraState.h"

class CReachLimitState :
    public CCameraState
{
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CReachLimitState);
public:
    CReachLimitState();
    ~CReachLimitState();
};

