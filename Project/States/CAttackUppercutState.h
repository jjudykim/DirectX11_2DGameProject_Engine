#pragma once
#include "CPlayerState.h"

class CAttackUppercutState :
    public CPlayerState
{
private:
    float     m_OriginJumpSpeed;

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackUppercutState);

public:
    CAttackUppercutState();
    ~CAttackUppercutState();
};

