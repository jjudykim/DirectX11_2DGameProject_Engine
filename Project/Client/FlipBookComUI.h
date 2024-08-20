#pragma once
#include "ComponentUI.h"

class FlipBookComUI :
    public ComponentUI
{
private:
    class CFlipBookComponent*   m_FBCom;
    Ptr<CFlipBook>              m_CurFB;
    Ptr<CFlipBook>              m_SelectFB;
    Ptr<CSprite>                m_CurSprite;

    UINT                        m_UIHeight;
    
    float                       m_AccTime;
    int                         m_ListIdx;
    int                         m_CurFrmIdx;
    bool                        m_Repeat;
    bool                        m_AddMode;
    bool                        m_Playing;

private:
    virtual void Init() override;
    virtual void Update() override;

    void SetCurFlipBookInfo();
    void FlipBookList();
    void EditFlipBookList();
    void CurFlipBookInfo();

    void AddFlipBookPopup();
    void SelectFlipBook(DWORD_PTR _AssetName);
    

private:
    void ShowFlipBookSprite(Ptr<CFlipBook> _CurFlipBook, int _CurIndex);

public:
    FlipBookComUI();
    ~FlipBookComUI();
};

