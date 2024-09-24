#pragma once
#include "Engine\CScript.h"

enum class PHASE_STEP
{
    BEGIN,
    PUZZLE,
    ATTACK,
    SPAWN,
    END,
};

struct TriggerPuzzleInfo
{
    CGameObject*      m_PuzzleObject;
    int               m_Priority;
    bool              m_IsActivated;
    bool              m_IsSetting;
};

class CBossScript :
    public CScript
{
private:
    TriggerPuzzleInfo       m_Puzzles[3];
    UINT                    m_CurPhaseStep;
    int                     m_BossHP;
    int                     m_CurPhase;
    bool                    m_IsBossDead;
    
    
    // Player & Attack Info
    CGameObject*            m_Player;
    UINT                    m_PlayerAttackID;
    int                     m_DamageReactStandard;
    int                     m_ShootProjectile;       // 0 : None, 1 : Arc, 2 : Bouncing
    
    // Puzzle
    bool                    m_IsPuzzleOn;
    int                     m_PuzzleState;            // 0 : Onging, 1 : Failed, 2 : Succeed
    int                     m_ActivePuzzleCount;
    float                   m_Time;
    float                   m_PuzzleSize;
    bool                    m_IsAbleResetPuzzle;
                            
    wchar_t                 m_PriorityInfo[256];
    wchar_t                 m_BossHPInfo[256];

    // Spawn
    vector<CGameObject*>    m_vecSpawnedMonsters;
    int                     m_MonsterSpawnState;        // 0 : None, 1 : Begin, 2 : Spawned, 3 : Terminated
    int                     m_MonsterCount;
    Ptr<CPrefab>            m_MonsterPref0;
    Ptr<CPrefab>            m_MonsterPref1;
    Ptr<CPrefab>            m_ArcProjectile;
    Ptr<CPrefab>            m_BouncingProjectile;
    

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

private:
    void InitAllPuzzles();
    void TermAllPuzzles();
    void ResetPuzzles();
    void PuzzlesPrioritySet();

    void DamagedByPlayerWeapon();
    
    void SpawnMonsters(int _index, int _MonsterType);

public:
    CLONE(CBossScript);
    CBossScript();
    ~CBossScript();
};

