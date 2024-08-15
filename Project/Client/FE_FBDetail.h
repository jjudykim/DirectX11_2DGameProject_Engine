#pragma once
#include "FE_Sub.h"

class FE_FBDetail :
    public FE_Sub
{
public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FE_FBDetail();
    ~FE_FBDetail();
};

