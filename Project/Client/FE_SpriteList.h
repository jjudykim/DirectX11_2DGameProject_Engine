#pragma once
#include "FE_Sub.h"

class FE_SpriteList :
    public FE_Sub
{
private:
    vector<Ptr<CSprite>> m_vecAddedSprite;

    int                  m_ListIndex;
    bool                 m_IsActive;


public:
    virtual void Init() override;
    virtual void Update() override;

private:
    void ShowList();

public:
    FE_SpriteList();
    ~FE_SpriteList();
};

