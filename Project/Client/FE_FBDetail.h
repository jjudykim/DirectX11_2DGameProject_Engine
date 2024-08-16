#pragma once
#include "FE_Sub.h"

class FE_FBDetail :
    public FE_Sub
{
private:
    Ptr<CFlipBook>     m_CurFlipBook;
    Ptr<CSprite>       m_CurSprite;

    int                m_FPS;
    bool               m_Repeat;
    int                m_CurSpriteIndex;
    int                m_MaxSpriteIndex;

    bool               m_Mode[2];      // 0 : CreateMode / 1 : EditMode
    bool               m_EditMode;          

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    int GetCurSpriteIndex() { return m_CurSpriteIndex; }
    Ptr<CFlipBook> GetCurFlipBook() { return m_CurFlipBook; }
    Ptr<CSprite> GetCurSprite() { return m_CurSprite; }
    bool* GetCurMode() { return m_Mode; }

    void SetCurSpriteIndex(int _SpriteIndex) { m_CurSpriteIndex = _SpriteIndex; }
    void SetCurSprite(Ptr<CSprite> _Sprite) { m_CurSprite = _Sprite; }

private:
    void SetFlipBookData();
    void SelectMode();
    void FlipBookPlay();
    void FlipBookInfo();
    void SpriteInfo();

private:
    void SelectFlipBook(DWORD_PTR _AssetName);

public:
    FE_FBDetail();
    ~FE_FBDetail();
};

