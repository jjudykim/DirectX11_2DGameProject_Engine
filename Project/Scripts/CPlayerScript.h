#pragma once
#include <Engine/CScript.h>

class CPlayerScript :
    public CScript
{
private:
    CGameObject*         m_Weapon;

    float                m_Speed;
    float                m_GravityAccel;
    UNITVEC_TYPE         m_Dir;
                         
    Ptr<CTexture>        m_Texture;
    Ptr<CPrefab>         m_MissilePref;

    map<UINT, UNITVEC_TYPE> m_mapNonePlatform;

    int                  m_OverlapPLTCount;
    int                  m_JumpCount;
    int                  m_ReachMapLimit;
    int                  m_ReachNoPltCol;
    int                  m_GodMode;

    float                m_JumpSpeed;
    float                m_Friction;
    float                m_MaxWalkSpeed;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetGodMode(bool _Mode) { _Mode == true ? m_GodMode = 1 : m_GodMode = 0; }

private:
    void GroundLogic();
    void AirLogic();
    void CorrectionYByFlatform(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider);

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

