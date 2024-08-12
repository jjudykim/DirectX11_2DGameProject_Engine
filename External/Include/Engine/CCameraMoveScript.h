#pragma once
#include "CScript.h"
class CCameraMoveScript :
    public CScript
{
private:
    float m_CamSpeed;

public:
    virtual void Tick() override;

private:
    void OrthoGraphicMove();
    void PerspectiveMove();

public:
    CLONE(CCameraMoveScript);
    CCameraMoveScript();
    ~CCameraMoveScript();
};

