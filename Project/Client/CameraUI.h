#pragma once
#include "ComponentUI.h"
class CameraUI :
    public ComponentUI
{
private:
    bool m_ShowLayerCheck;

private:
    virtual void Update() override;

private:
    void LayerCheck();
    void Projection();
    void ProjectionInfo();

public:
    CameraUI();
    ~CameraUI();
};

