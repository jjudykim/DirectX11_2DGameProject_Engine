#pragma once
#include "CCameraState.h"

class CReachLimitState :
    public CCameraState
{
private:
    Vec3             m_LimitTargetPos;
    Vec3             m_LimitTargetScale;
           
    float            m_springConstant;
    float            m_dampingFactor;
                     
    Vec3             m_Velocity;

    int              m_ReachMapLimit;

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CReachLimitState);
public:
    CReachLimitState();
    ~CReachLimitState();
};

