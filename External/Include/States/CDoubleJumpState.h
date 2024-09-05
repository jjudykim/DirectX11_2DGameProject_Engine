#pragma once
#include "CPlayerState.h"
class CDoubleJumpState :
    public CPlayerState
{
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CDoubleJumpState);
public:
    CDoubleJumpState();
    ~CDoubleJumpState();
};

