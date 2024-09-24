#pragma once
#include "Engine\CScript.h"

class CBouncingProjectileScript :
    public CScript
{
    CGameObject*        m_Player;
    float               m_DestinationPosX;
    float               m_VelocityX;

    bool                m_IsMaxSize;
    bool                m_IsExploding;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CBouncingProjectileScript);
    CBouncingProjectileScript();
    ~CBouncingProjectileScript();
};

