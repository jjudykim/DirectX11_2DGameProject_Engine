#pragma once
#include "Engine\CScript.h"

class CBackgroundMoveScript :
    public CScript
{
    UINT           m_BackgroundLayer;

    Vec3           m_OriginPos;
                   
    CGameObject*   m_MainCamera;


public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) {};

public:
    CLONE(CBackgroundMoveScript);
    CBackgroundMoveScript();
    ~CBackgroundMoveScript();
};

