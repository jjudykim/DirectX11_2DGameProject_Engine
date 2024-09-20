#pragma once
#include "CBossState.h"
class CBossJumpBackState :
    public CBossState
{
private:
    float             m_Time;
    bool              m_ResetTime;
    bool              m_PassPeakPoint;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossJumpBackState)

public:
    CBossJumpBackState();
    ~CBossJumpBackState();
};

