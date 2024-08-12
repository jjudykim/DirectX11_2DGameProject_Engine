#pragma once
#include "CComponent.h"

class CFlipBook;

class CFlipBookComponent :
    public CComponent
{
private:
    vector<Ptr<CFlipBook>>      m_vecFlipBook;      // 모든 FlipBook 목록

    Ptr<CFlipBook>              m_CurFlipBook;      // 현재 재생중인 FlipBook
    Ptr<CSprite>                m_CurFrmSprite;     // 현재 재생중인 FlipBook이 재생중인 Sprite
    int                         m_CurFrmIdx;        // 현재 재생중인 FlipBook이 재생중인 Sprite의 Index
    float                       m_FPS;              // 현재 재생중인 FlipBook의 초당 프레임 수

    float                       m_AccTime;          // 누적 시간값 체크
    bool                        m_Repeat;           // 재생 반복 여부
    bool                        m_Finish;           // FlipBook이 모두 재생되었는지에 대한 여부

public:
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }

public:
    void AddFlipBook(int _Idx, Ptr<CFlipBook> _FliPBook);
    Ptr<CFlipBook> FindFlipBook(const wstring& _Key);
    void Play(int _FlipBookIdx, float _FPS, bool _Repeat);
    
public:
    void Reset();
    void Binding();
    static void Clear();

public:
    CLONE(CFlipBookComponent);
    CFlipBookComponent();
    CFlipBookComponent(CFlipBookComponent& _Origin);
    ~CFlipBookComponent();
};

