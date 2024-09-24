#pragma once
#include "Engine\CState.h"

class CBossState :
    public CState
{
protected:
    CGameObject*            m_Boss;

    int                     m_CurPhase;
    int                     m_PhaseStep;
    int                     m_MonsterSpawnState;
                            
    CFlipBookComponent*     m_FBCom;
    Ptr<CFlipBook>          m_CurFB;
    int                     m_FBIdx;


public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CBossState();
    ~CBossState();
};

