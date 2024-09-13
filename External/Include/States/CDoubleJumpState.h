#pragma once
#include "CPlayerState.h"
class CDoubleJumpState :
    public CPlayerState
{
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CDoubleJumpState);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CDoubleJumpState();
    ~CDoubleJumpState();
};

