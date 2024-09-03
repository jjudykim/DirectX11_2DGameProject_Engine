#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    float            m_Speed;
    UNITVEC_TYPE     m_Dir;

    Ptr<CTexture>    m_Texture;
    Ptr<CPrefab>     m_MissilePref;

    int              m_OverlapPLTCount;
    int              m_ReachMapLimit;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    float GetSpeed() { return m_Speed; }

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    
public:
    CLONE(CPlayerScript);
    CPlayerScript();
    ~CPlayerScript();
};

