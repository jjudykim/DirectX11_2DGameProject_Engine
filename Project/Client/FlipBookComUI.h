#pragma once
#include "ComponentUI.h"

class FlipBookComUI :
    public ComponentUI
{
private:
    class CFlipBookComponent*   m_FBCom;
    Ptr<CFlipBook>              m_CurFB;
    Ptr<CSprite>                m_CurSprite;

    UINT                        m_UIHeight;
    
    float                       m_AccTime;
    UINT                        m_CurFrmIdx;
    bool                        m_Repeat;
    bool                        m_Playing;

private:
    virtual void Update() override;

    void SelectFlipBook(DWORD_PTR _AssetName);

private:
    void ShowFlipBookSprite(Ptr<CFlipBook> _CurFlipBook, int _CurIndex);

public:
    FlipBookComUI();
    ~FlipBookComUI();
};

