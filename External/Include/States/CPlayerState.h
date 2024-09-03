#pragma once

#include "Engine/CState.h"
#include "Engine/CGameObject.h"

class CPlayerState :
    public CState
{
protected:
    CGameObject*         m_Player;

    float                m_Speed;
    UNITVEC_TYPE         m_Dir;
    bool                 m_ReachMapLimit;

    CFlipBookComponent*  m_FBCom;
    Ptr<CFlipBook>       m_CurFB;
    int                  m_FBIdx;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPlayerState();
    ~CPlayerState();
};

