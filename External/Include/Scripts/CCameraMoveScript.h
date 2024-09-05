#pragma once
#include <Engine/CScript.h>

class CCameraMoveScript :
    public CScript
{
private:
    UNITVEC_TYPE     m_Dir;
    float            m_CamSpeed;

    Vec3             m_PlayerPos;
    Vec3             m_PlayerVelocity;
    Vec3             m_PlayerGravityVelocity;

    Vec3             m_LimitTargetPos;
    Vec3             m_LimitTargetScale;
                     
    bool             m_IsReachLimit;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();
};

