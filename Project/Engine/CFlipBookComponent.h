#pragma once
#include "CComponent.h"

class CFlipBook;

class CFlipBookComponent :
    public CComponent
{
private:
    vector<Ptr<CFlipBook>>      m_vecFlipBook;      // ��� FlipBook ���

    Ptr<CFlipBook>              m_CurFlipBook;      // ���� ������� FlipBook
    Ptr<CSprite>                m_CurFrmSprite;     // ���� ������� FlipBook�� ������� Sprite
    int                         m_CurFrmIdx;        // ���� ������� FlipBook�� ������� Sprite�� Index

    float                       m_AccTime;          // ���� �ð��� üũ
    bool                        m_Repeat;           // ��� �ݺ� ����
    bool                        m_Finish;           // FlipBook�� ��� ����Ǿ������� ���� ����

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

