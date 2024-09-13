#pragma once
#include "CPlayerState.h"

class CAttackLight1State :
    public CPlayerState
{
private:
    bool      m_ConnectNextAttack;

private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackLight1State);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CAttackLight1State();
    ~CAttackLight1State();
};

