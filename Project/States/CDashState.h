#pragma once
#include "CPlayerState.h"

class CDashState :
    public CPlayerState
{
private:
    float       m_MaxWalkSpeed;

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CDashState);

public:
    virtual bool IsAttackState() { return m_IsAttackState; }

public:
    CDashState();
    ~CDashState();
};

