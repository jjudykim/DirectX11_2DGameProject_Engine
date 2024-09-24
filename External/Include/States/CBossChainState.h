#pragma once
#include "CBossState.h"
class CBossChainState :
    public CBossState
{
private:
    int            m_MonsterSpawnState;
    CGameObject*   m_BossChain;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

    CLONE_DISABLE(CBossChainState)

public:
    CBossChainState();
    ~CBossChainState();
};

