#pragma once
#include "CEntity.h"
#include "CFSM.h"

class CGameObject;

class CState :
    public CEntity
{
private:
    CFSM*       m_Owner;

protected:
    CGameObject* GetTargetObject() { return m_Owner->GetOwner(); }
    CFSM* GetFSM() { return m_Owner; }

public:
    virtual void Set() = 0;
    virtual void Enter() = 0;
    virtual void FinalTick() = 0;
    virtual void Exit() = 0;

public:
    CState();
    ~CState();

    friend class CFSM;
};

