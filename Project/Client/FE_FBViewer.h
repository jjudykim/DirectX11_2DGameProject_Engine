#pragma once
#include "FE_Sub.h"

class FE_FBViewer :
    public FE_Sub
{
private:
    ImVec2       m_ImageRectMin;
    ImVec2       m_ViewerSize;

private:
    void DrawGrid();

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FE_FBViewer();
    ~FE_FBViewer();
};

