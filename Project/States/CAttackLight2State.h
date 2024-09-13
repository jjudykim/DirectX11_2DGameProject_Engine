#pragma once
#include "CPlayerState.h"

class CAttackLight2State :
    public CPlayerState
{
private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackLight2State);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CAttackLight2State();
    ~CAttackLight2State();
};

