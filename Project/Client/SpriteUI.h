#pragma once
#include "AssetUI.h"

class SpriteUI :
    public AssetUI
{
public:
    Ptr<CTexture>          m_AtlasTex;
    Ptr<CSprite>           m_CurSprite;

    ImVec2                 m_AtlasSize;

    ImVec2                 m_SpriteLT;
    ImVec2                 m_SpriteSlice;
    ImVec2                 m_SpriteBG;

public:
    virtual void Update() override;

private:
    void SpriteInfo();
    void AtlasInfo();

private:
    void SetSpriteInfo();

public:
    SpriteUI();
    ~SpriteUI();
};

