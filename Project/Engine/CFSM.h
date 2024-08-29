#pragma once
#include "CComponent.h"

class CState;

class CFSM :
    public CComponent
{
private:
    map<wstring, CState*>      m_mapState;
    CState*                    m_CurState;
    CState*                    m_PrevState;

public:
    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }
    

public:
    void SetState();
    void AddState(const wstring& _Key, CState* _State);
    CState* FindState(const wstring& _Key);
    void ChangeState(const wstring& _NextStateKey);

public:
    virtual void FinalTick() override;

    virtual void LoadFromFile(FILE* _File) override;
    virtual void SaveToFile(FILE* _File) override;
public:
    CLONE(CFSM);

    CFSM();
    ~CFSM();
};

