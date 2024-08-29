#pragma once

#include "Engine/CState.h"

class CPlayerState :
    public CState
{
private:
    CGameObject*        m_Player;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPlayerState();
    ~CPlayerState();
};

