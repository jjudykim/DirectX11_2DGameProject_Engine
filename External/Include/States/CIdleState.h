#pragma once
#include "CPlayerState.h"

class CIdleState :
    public CPlayerState
{

private:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CIdleState);

public:
    CIdleState();
    ~CIdleState();
};

