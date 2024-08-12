#pragma once
#include "AssetUI.h"

class PrefabUI :
    public AssetUI
{
    virtual void Update() override;

public:
    PrefabUI();
    ~PrefabUI();
};

