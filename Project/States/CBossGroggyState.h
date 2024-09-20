#pragma once
#include "CBossState.h"
class CBossGroggyState :
    public CBossState
{
public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossGroggyState)

public:
    CBossGroggyState();
    ~CBossGroggyState();
};

