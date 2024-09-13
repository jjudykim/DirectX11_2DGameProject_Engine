#pragma once
#include "CPlayerState.h"

class CRunState :
    public CPlayerState
{
private:
    float      m_Friction;

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CRunState);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CRunState();
    ~CRunState();
};

