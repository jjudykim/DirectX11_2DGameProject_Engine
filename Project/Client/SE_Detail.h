#pragma once
#include "SE_Sub.h"

class SE_Detail :
    public SE_Sub
{
private:
    Ptr<CTexture>          m_AtlasTex;
    Vec2                   m_AtlasResolution;

    string                 m_SpriteBundleName;
    Ptr<CSprite>           m_CurSprite;
    vector<Ptr<CSprite>>   m_vecAddSprite;

    int                    m_CurIndex;
    bool                   m_IsSelectedSprite;
    bool                   m_IsRegistedSprite;
    ImVec2                 m_Background;

private:
    void AtlasInfo();
    void SpriteBundleInfo();
    void SelectSpriteInfo();
    void SpriteList();
    
    bool IsChangedSelectSprite(Ptr<CSprite> _NewSprite);
    bool IsChangedSelectSprite(ImVec2 _SpriteLT, ImVec2 _SpriteRB);
    bool IsAddedSprite(UINT _ID);
    void CalcBackgroundSize();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);
    void SetSelectedSprite(bool _b) { m_IsSelectedSprite = false; }

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI);

public:
    SE_Detail();
    ~SE_Detail();
};

