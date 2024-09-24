#pragma once
#include "CBossState.h"
class CBossAttackState :
    public CBossState
{
    int         m_ShootProjectile;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossAttackState)

public:
    CBossAttackState();
    ~CBossAttackState();
};

