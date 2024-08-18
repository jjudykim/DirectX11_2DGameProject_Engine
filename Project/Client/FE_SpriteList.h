#pragma once
#include "FE_Sub.h"

class FE_SpriteList :
    public FE_Sub
{
private:
    vector<Ptr<CSprite>> m_vecAddedSprite;

    int                  m_ListIndex;
    int                  m_PrevIndex;
    bool                 m_IsActive;


public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void ShowList();
    void SelectSprite(DWORD_PTR _AssetName);

public:
    void SetSpriteToList(vector<Ptr<CSprite>>& _vecSprite);

    void SetCurSpriteIndex(int _index) { m_ListIndex = _index; }

    int GetCurSpriteIndex() { return m_ListIndex; }
    int GetSpriteCount() { return m_vecAddedSprite.size(); }
    Ptr<CSprite> GetCurSprite() { return m_vecAddedSprite[m_ListIndex]; }

public:
    FE_SpriteList();
    ~FE_SpriteList();

    friend class FE_FBDetail;
};

