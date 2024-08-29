#pragma once
#include "CComponent.h"

enum class DATA_TYPE
{
    INT,
    FLOAT,
    VEC2,
    WSTRING,
    UNITVEC_TYPE,
};

struct BlackboardData
{
    DATA_TYPE  Type;
    void*      pData;
};

class CState;

class CFSM :
    public CComponent
{
private:
    map<wstring, BlackboardData>      m_mapData;
    map<wstring, CState*>             m_mapState;
    CState*                           m_CurState;
    CState*                           m_PrevState;

public:
    CState* GetCurState() { return m_CurState; }
    CState* GetPrevState() { return m_PrevState; }
    
public:
    void SetState();
    void AddState(const wstring& _Key, CState* _State);
    CState* FindState(const wstring& _Key);
    void ChangeState(const wstring& _NextStateKey);
    void SetBlackboardData(const wstring& _DataKey, DATA_TYPE _Type, void* _pData);

    template<typename T>
    T GetBlackboardData(const wstring& _DataKey);

public:
    virtual void FinalTick() override;

public:
    virtual void LoadFromFile(FILE* _File) override;
    virtual void SaveToFile(FILE* _File) override;

public:
    CLONE(CFSM);

    CFSM();
    ~CFSM();
};

template<typename T>
inline T CFSM::GetBlackboardData(const wstring& _DataKey)
{
    map<wstring, BlackboardData>::iterator iter = m_mapData.find(_DataKey);

    assert(iter != m_mapData.end());

    if (std::is_same_v<int, T>)
    {
        assert(DATA_TYPE::INT == iter->second.Type);
        return *((T*)iter->second.pData);
    }

    if (std::is_same_v<float, T>)
    {
        assert(DATA_TYPE::FLOAT == iter->second.Type);
        return *((T*)iter->second.pData);
    }

    if (std::is_same_v<Vec2, T>)
    {
        assert(DATA_TYPE::VEC2 == iter->second.Type);
        return *((T*)iter->second.pData);
    }

    if (std::is_same_v<UNITVEC_TYPE, T>)
    {
        assert(DATA_TYPE::UNITVEC_TYPE == iter->second.Type);
        return *((T*)iter->second.pData);
    }
}
