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
    int               m_Time;
};

class CBossScript :
    public CScript
{
private:
    TriggerPuzzleInfo   m_Puzzles[3];
    PHASE_STEP          m_CurPhaseStep;
    int                 m_BossHP;
    int                 m_CurPhase;
                        
    int                 m_IsPuzzleOn;
    int                 m_ActivePuzzleCount;
    float               m_Time;
    float               m_PuzzleSize;
    bool                m_IsPuzzleFailed;
    bool                m_ResettingPuzzle;


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
    void ResetPuzzles();
    void PuzzlesPrioritySet();

public:
    CLONE(CBossScript);
    CBossScript();
    ~CBossScript();
};

