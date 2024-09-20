#pragma once
#include "Engine\CScript.h"

class CBossPuzzleScript :
    public CScript
{
    int          m_Sequence;
    bool         m_IsActivated;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CBossPuzzleScript);
    CBossPuzzleScript();
    ~CBossPuzzleScript();
};

