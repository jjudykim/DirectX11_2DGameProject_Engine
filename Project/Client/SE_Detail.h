#pragma once
#include "SE_Sub.h"

class SE_Detail :
    public SE_Sub
{
private:
    Ptr<CTexture>          m_AtlasTex;
    Vec2                   m_AtlasResolution;

    Ptr<CSprite>           m_CurSprite;
    vector<Ptr<CSprite>>   m_vecAddSprite;

    int                    m_CurIndex;
    bool                   m_IsSelectedSprite;
    ImVec2                 m_Background;

private:
    void Atlas();
    void AtlasInfo();
    void SelectSpriteInfo();
    void SpriteList();
    
    bool IsChangedSelectSprite(Ptr<CSprite> _NewSprite);
    bool IsChangedSelectSprite(ImVec2 _SpriteLT, ImVec2 _SpriteRB);
    bool IsAddedSprite(UINT _ID);
    void CalcBackgroundSize();

public:
    void SetAtlasTex(Ptr<CTexture> _Tex);

public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void SelectTexture(DWORD_PTR _ListUI);

public:
    SE_Detail();
    ~SE_Detail();
};

