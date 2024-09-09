#pragma once
#include "CPlayerState.h"

class CAttackSmashState :
    public CPlayerState
{
private:
    Ptr<CFlipBook>   m_SmashFB[3];
    int              m_SmashFBIdx[3];

    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CAttackSmashState);

public:
    CAttackSmashState();
    ~CAttackSmashState();
};

