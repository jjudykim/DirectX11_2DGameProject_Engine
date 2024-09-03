#pragma once
#include "ComponentUI.h"

class FSMUI :
    public ComponentUI
{
private:
    class CFSM*       m_FSM;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FSMUI();
    ~FSMUI();
};

