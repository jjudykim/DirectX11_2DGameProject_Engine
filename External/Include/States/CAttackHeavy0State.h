#pragma once
#include "CPlayerState.h"

class CAttackHeavy0State :
    public CPlayerState
{
private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackHeavy0State);

public:
    CAttackHeavy0State();
    ~CAttackHeavy0State();
};

