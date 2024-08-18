#pragma once
#include "FE_Sub.h"

class FE_FBDetail :
    public FE_Sub
{
private:
    Ptr<CFlipBook>     m_CurFlipBook;
    Ptr<CSprite>       m_CurSprite;
    Ptr<CTexture>      m_AtlasTex;

    string             m_CurFlipBookName;
    string             m_CurSpriteName;
    int                m_FPS;
    Vec2               m_Offset;
    bool               m_Repeat;
    int                m_CurSpriteIndex;
    int                m_MaxSpriteIndex;

    bool               m_Mode[2];      // 0 : CreateMode / 1 : EditMode
    bool               m_IsActive;
    bool               m_IsPlaying;
    float              m_AccTime;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    int GetCurSpriteIndex() { return m_CurSpriteIndex; }
    Ptr<CFlipBook> GetCurFlipBook() { return m_CurFlipBook; }
    Ptr<CSprite> GetCurSprite() { return m_CurSprite; }
    Ptr<CTexture> GetAtlasTex() { return m_AtlasTex; }
    bool* GetCurMode() { return m_Mode; }

    void SetCurSprite(Ptr<CSprite> _Sprite)
    { 
        m_CurSprite = _Sprite;
        SetSpriteData();
    }

private:
    void SetFlipBookData();
    void SetSpriteData();

    void SelectMode();
    void FlipBookInfo();
    void SpriteInfo();
    void SaveFlipBook();
    void FlipBookPlay();

private:
    void SelectFlipBook(DWORD_PTR _AssetName);

public:
    FE_FBDetail();
    ~FE_FBDetail();
};

