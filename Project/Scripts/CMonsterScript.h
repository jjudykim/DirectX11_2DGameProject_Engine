#pragma once
#include "Engine/CScript.h"

class CMonsterScript :
    public CScript
{
private:
    UINT           m_MonsterType;
                   
    Vec3           m_OriginScale;
    int            m_MonsterHP;
    int            m_MonsterAttackPower;
    int            m_DamageReactStandard;

    CGameObject*   m_Player;
    UINT           m_PlayerAttackID;

    int            m_OverlapPLTCount;
    int            m_OverlapWithPlayer;
    int            m_IsLimitMap;

    bool           m_Alive;
    bool           m_IsAttacking;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

private:
    void DamagedByPlayerWeapon();

public:
    CLONE(CMonsterScript);
    CMonsterScript();
    ~CMonsterScript();
};

