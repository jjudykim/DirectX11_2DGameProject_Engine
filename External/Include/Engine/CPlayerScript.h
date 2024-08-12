#pragma once
#include "CScript.h"

class CPlayerScript :
    public CScript
{

private:
    float m_Speed;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

