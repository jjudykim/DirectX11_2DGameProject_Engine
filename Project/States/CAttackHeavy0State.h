#pragma once
#include "CPlayerState.h"

class CAttackHeavy0State :
    public CPlayerState
{
private:
    bool m_ConnectNextAttack;

private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackHeavy0State);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CAttackHeavy0State();
    ~CAttackHeavy0State();
};

