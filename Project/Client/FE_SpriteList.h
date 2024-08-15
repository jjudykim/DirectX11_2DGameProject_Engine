#pragma once
#include "FE_Sub.h"

class FE_SpriteList :
    public FE_Sub
{
public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FE_SpriteList();
    ~FE_SpriteList();
};

