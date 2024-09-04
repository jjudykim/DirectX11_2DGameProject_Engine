#pragma once
#include "CCameraState.h"

class CChasingPlayerState :
    public CCameraState
{
private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CChasingPlayerState);
public:
    CChasingPlayerState();
    ~CChasingPlayerState();
};

