#pragma once
#include "CCameraState.h"
class CBossModeState :
    public CCameraState
{
private:
    float      m_Friction;
    float      m_MaxWalkSpeed;

    int        m_ReachMapLimit;

private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossModeState);
public:
    CBossModeState();
    ~CBossModeState();
};