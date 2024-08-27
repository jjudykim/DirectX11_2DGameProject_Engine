#pragma once
#include "SE_Sub.h"

class SE_AtlasView :
    public SE_Sub
{
private:
    Ptr<CTexture>    m_AtlasTex;

    ImVec2           m_ImageRectMin;        // Image ������ Left Top Position
    ImVec2           m_MousePos;

    ImVec2           m_MouseLT;             // Mouse ��ġ�� �ش��ϴ� Texture�� Left Top
    ImVec2           m_MouseRB;             // Mouse ��ġ�� �ش��ϴ� Texture�� Right Bottom

    set<Vec2>        m_PixelID;             // ��ϵ� �� �ִ� Pixel���� Ȯ��
                    
    float            m_WidthSize;
    float            m_WheelScale;
    float            m_Ratio;               // ���� Resolution ��� Output Image ����

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

