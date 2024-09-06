#pragma once
#include "CPlayerState.h"

class CAttackHeavy1State :
    public CPlayerState
{
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackHeavy1State);

public:
    CAttackHeavy1State();
    ~CAttackHeavy1State();
};

