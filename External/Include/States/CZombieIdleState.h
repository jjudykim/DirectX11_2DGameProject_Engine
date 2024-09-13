#pragma once
#include "CMonsterState.h"

class CZombieIdleState :
    public CMonsterState
{
private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE_DISABLE(CZombieIdleState);
    CZombieIdleState();
    ~CZombieIdleState();
};

