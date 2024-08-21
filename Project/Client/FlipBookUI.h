#pragma once
#include "AssetUI.h"

class FlipBookUI :
    public AssetUI
{
private:
    Ptr<CFlipBook>         m_CurFB;
    Ptr<CSprite>           m_CurSprite;
    Ptr<CTexture>          m_CurAtlas;

    int                    m_CurFrmIdx;

    ImVec2                 m_FrameSize;
    float                  m_AccTime;
    bool                   m_Playing;
    bool                   m_Repeat;

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void CurFlipBookInfo();
    void ShowFlipBookSprite();

public:
    FlipBookUI();
    ~FlipBookUI();
};

