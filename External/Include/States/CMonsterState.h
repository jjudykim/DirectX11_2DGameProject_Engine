#pragma once
#include "Engine/CState.h"

class CMonsterState :
    public CState
{   
protected:
    UINT                   m_MonsterType;
    CGameObject*           m_Monster;
    CGameObject*           m_Missile;
                           
    CFlipBookComponent*    m_FBCom;
    Ptr<CFlipBook>         m_CurFB;
    int                    m_FBIdx;

    float                  m_Speed;

    int                    m_OverlapWithPlayer;
    int                    m_IsLimitMap;
    UNITVEC_TYPE           m_TargetDir;
    float                  m_AttackArea;

    bool                   m_IsAttacking;

public:
    virtual void Set() override;
    virtual void Enter() override;
    virtual void FinalTick() override;
    virtual void Exit() override;

public:
    CMonsterState();
    ~CMonsterState();
};

