#pragma once
#include "CPlayerState.h"

class CJumpState :
    public CPlayerState
{
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CJumpState);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CJumpState();
    ~CJumpState();
};

