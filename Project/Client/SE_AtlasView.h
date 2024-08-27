#pragma once
#include "SE_Sub.h"

class SE_AtlasView :
    public SE_Sub
{
private:
    Ptr<CTexture>    m_AtlasTex;

    ImVec2           m_ImageRectMin;        // Image 위젯의 Left Top Position
    ImVec2           m_MousePos;

    ImVec2           m_MouseLT;             // Mouse 위치에 해당하는 Texture의 Left Top
    ImVec2           m_MouseRB;             // Mouse 위치에 해당하는 Texture의 Right Bottom

    set<Vec2>        m_PixelID;             // 등록된 적 있는 Pixel인지 확인
                    
    float            m_WidthSize;
    float            m_WheelScale;
    float            m_Ratio;               // 실제 Resolution 대비 Output Image 비율

public:
    ImVec2 GetSpriteLT() { return m_MouseLT; }
    void   SetMouseLT(ImVec2 _LT) { m_MouseLT = _LT; }
    ImVec2 GetSpriteRB() { return m_MouseRB; }
    void   SetMouseRB(ImVec2 _RB) { m_MouseRB = _RB; }

    void SetAtlasTex(Ptr<CTexture> _Tex);
    void SetMouseLTRB(ImVec2 _LT, ImVec2 _RB);

private:
    void WheelCheck();
    void SelectCheck();
    void DrawSelectRect();
    void DrawSelectRect(ImVec2 _LT, ImVec2 _RT);
    void CalcSpriteSize(Vec2 _PixelPos);
    bool IsPixelOk(Vec2 _PixelPos);

public:
    virtual void Init() override;
    virtual void Update() override;

public:
    SE_AtlasView();
    ~SE_AtlasView();
};

