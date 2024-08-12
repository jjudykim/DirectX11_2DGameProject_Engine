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
    float                       m_FPS;              // ���� ������� FlipBook�� �ʴ� ������ ��

    float                       m_AccTime;          // ���� �ð��� üũ
    bool                        m_Repeat;           // ��� �ݺ� ����
    bool                        m_Finish;           // FlipBook�� ��� ����Ǿ������� ���� ����

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

