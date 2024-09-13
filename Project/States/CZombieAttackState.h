#pragma once
#include "CMonsterState.h"

class CZombieAttackState :
    public CMonsterState
{
    Vec3        m_RelativeScale;
    float       m_ProgressTime;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CLONE_DISABLE(CZombieAttackState);
    CZombieAttackState();
    ~CZombieAttackState();
};

