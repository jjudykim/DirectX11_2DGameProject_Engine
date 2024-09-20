#pragma once
#include "CBossState.h"
class CBossFallState :
    public CBossState
{
private:
    float     m_Time;
    bool      m_ResetTime;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossFallState)

public:
    CBossFallState();
    ~CBossFallState();
};

