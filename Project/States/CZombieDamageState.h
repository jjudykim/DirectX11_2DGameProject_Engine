#pragma once
#include "CMonsterState.h"

class CZombieDamageState :
    public CMonsterState
{
private:
    Vec3        m_RelativeScale;

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE_DISABLE(CZombieDamageState);
    CZombieDamageState();
    ~CZombieDamageState();
};

