#pragma once
#include "ComponentUI.h"

class Collider2DUI :
    public ComponentUI
{
private:
    CCollider2D*     m_ColliderCom;
    ImVec4           m_Scale;
    ImVec4           m_Offset;

    bool             m_Independent;
    bool             m_ShowLayerCheck;
    int              m_Ratio;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void ApplyColliderDetail();
    void LayerCollisionCheck();

public:
    Collider2DUI();
    ~Collider2DUI();
};

