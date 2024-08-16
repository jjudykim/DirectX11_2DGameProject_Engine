#pragma once
#include "FE_Sub.h"

class FE_FBViewer :
    public FE_Sub
{
private:
    Ptr<CSprite>    m_CurSprite;
    Ptr<CTexture>   m_AtlasTex;
    
                    
    ImVec2          m_ImageRectMin;
    ImVec2          m_ViewerSize;

private:
    void SetImage();
    void DrawGrid();

public:
    void SetCurSprite(Ptr<CSprite> _Sprite) { m_CurSprite = _Sprite; }

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FE_FBViewer();
    ~FE_FBViewer();
};

