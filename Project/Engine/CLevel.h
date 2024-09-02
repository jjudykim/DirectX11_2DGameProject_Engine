#pragma once
#include "CEntity.h"

class CLayer;
class CGameObject;

class CLevel :
    public CEntity
{
private:
    CLayer*        m_Layer[MAX_LAYER];
    LEVEL_STATE    m_State;
    wstring        m_LevelTitle;
    UINT           m_CollisionMatrix[MAX_LAYER];

public:
    void AddObject(int LayerIdx, CGameObject* _Object, bool _bMoveChild = false);
    void RegisterAsParent(int LayerIdx, CGameObject* _Object);
    CGameObject* FindObjectByName(const wstring& _Name);

public:
    void SetLevelTitle(const wstring& _Name) { m_LevelTitle = _Name; }
    void SetCollisionInfo(const UINT* _matrix)
    {
        for (int i = 0; i < MAX_LAYER; ++i)
        {
            m_CollisionMatrix[i] = _matrix[i];
        }
    }

    CLayer* GetLayer(int _LayerIdx) { return m_Layer[_LayerIdx]; }
    const UINT* GetCollisionInfo() { return m_CollisionMatrix; }
    LEVEL_STATE GetState() { return m_State; }
    const wstring& GetLevelTitle() { return m_LevelTitle; }
    

private:
    void ChangeState(LEVEL_STATE _NextState);

public:
    void Begin();         // Level이 시작되거나 / Level에 추가되는 순간
    void Tick();
    void FinalTick();

    void ClearObject();

public:
    virtual CLevel* Clone() { return new CLevel(*this); }
    CLevel();
    CLevel(const CLevel& _Origin);
    ~CLevel();

    friend class CTaskMgr;
};

