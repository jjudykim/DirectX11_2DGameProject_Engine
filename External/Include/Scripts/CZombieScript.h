#pragma once
#include "CMonsterScript.h"

class CZombieScript :
    public CMonsterScript
{
public:
    virtual void Begin() override;
    virtual void Tick() override;

public:
    CLONE(CZombieScript);
    CZombieScript();
    ~CZombieScript();
};

