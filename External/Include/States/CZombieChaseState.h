#pragma once
#include "CMonsterState.h"

class CZombieChaseState :
    public CMonsterState
{
public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE_DISABLE(CZombieChaseState);
    CZombieChaseState();
    ~CZombieChaseState();
};

