#pragma once
#include "ComponentUI.h"

class FSMUI :
    public ComponentUI
{
private:
    class CFSM*       m_FSM;

    vector<wstring>   m_vecStateWstr;

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    FSMUI();
    ~FSMUI();
};

