#pragma once
#include "CScript.h"

class CMissileScript :
    public CScript
{
private:
    float       m_Speed;

public:
    virtual void Tick() override;
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CMissileScript);
    CMissileScript();
    ~CMissileScript();
};

