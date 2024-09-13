#pragma once

#include "Engine/CState.h"
#include "Engine/CGameObject.h"

class CPlayerState :
    public CState
{
protected:
    CGameObject*         m_Player;
    CGameObject*         m_Weapon;

    int                  m_AttackPower;

    float                m_Speed;
    UNITVEC_TYPE         m_Dir;
    int                  m_ReachMapLimit;
    int                  m_ReachNoPlatformCol;

    CFlipBookComponent*  m_FBCom;
    Ptr<CFlipBook>       m_CurFB;
    int                  m_FBIdx;
    int                  m_WeaponFBIdx;

    int                  m_JumpCount;

    bool                 m_IsAttackState;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CPlayerState();
    ~CPlayerState();
};

