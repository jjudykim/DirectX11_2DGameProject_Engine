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

    float                       m_AccTime;          // 누적 시간값 체크
    bool                        m_Repeat;           // 재생 반복 여부
    bool                        m_Finish;           // FlipBook이 모두 재생되었는지에 대한 여부

public:
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    Ptr<CSprite> GetCurSprite() { return m_CurFrmSprite; }
    Ptr<CFlipBook> GetCurFlipBook() { return m_CurFlipBook; }
    const vector<Ptr<CFlipBook>>& GetVecFlipBook() { return m_vecFlipBook; }
    bool GetRepeat() { return m_Repeat; }
    bool GetFinish() { return m_Finish; }

    void EraseFlipBook(int _index) { m_vecFlipBook.erase(m_vecFlipBook.begin() + _index); }
    void SetCurFlipBook(Ptr<CFlipBook> _FlipBook) { m_CurFlipBook = _FlipBook; }
    void SetCurFrame(int _index) { m_CurFrmSprite = m_CurFlipBook->GetSprite(_index); m_CurFrmIdx = _index; }
    void SetRepeat(bool _Repeat) { m_Repeat = _Repeat; }
    void SetFinish(bool _Finish) { m_Finish = _Finish; }

public:
    void AddFlipBook(int _Idx, Ptr<CFlipBook> _FliPBook);
    Ptr<CFlipBook> FindFlipBook(const wstring& _Key);
    int FindFlipBookIndex(const wstring& _Key);
    void Play(int _FlipBookIdx, bool _Repeat);
    
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

