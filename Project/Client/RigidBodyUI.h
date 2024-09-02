#pragma once
#include "ComponentUI.h"

class RigidBodyUI :
    public ComponentUI
{
public:
    virtual void Update() override;

public:
    RigidBodyUI();
    ~RigidBodyUI();
};

