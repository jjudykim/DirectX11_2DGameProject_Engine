#pragma once
#include "CComponent.h"

enum PROJ_TYPE
{
    ORTHOGRAPHIC,
    PERSPECTIVE,
};

class CCamera :
    public CComponent
{
private:
    int                    m_Priority;         // ī�޶��� �켱������ ���ϱ� ���� ���
    UINT                   m_LayerCheck;       // ���ϴ� ���̾ ��� ���� ���

    PROJ_TYPE              m_ProjType;

    float                  m_Width;
    float                  m_Height;
    float                  m_AspectRatio;      // ��Ⱦ��
    float                  m_Far;              // Near(�ּҰŸ�) & Far(�ִ�Ÿ�)
    float                  m_FOV;              // Field Of View
    float                  m_ProjectionScale;  

    Matrix                 m_matView;          // View ���
    Matrix                 m_matProj;          // ���� ���

    vector<CGameObject*>   m_vecOpaque;        // ������
    vector<CGameObject*>   m_vecMasked;        // ������, ����
    vector<CGameObject*>   m_vecTransparent;   // ����, ������
    vector<CGameObject*>   m_vecParticles;     // ����, ������, ���� Ÿ��
    vector<CGameObject*>   m_vecPostProcess;   // ��ó�� ������Ʈ
    vector<CGameObject*>   m_vecUI;            // UI

public:
    void SetPriority(int _Priority) { m_Priority = _Priority; }
    void SetLayer(UINT _LayerIdx, bool _bCheck)
    {
        if (_bCheck)
            m_LayerCheck |= (1 << _LayerIdx);
        else
            m_LayerCheck &= ~(1 << _LayerIdx);
    }
    void SetLayerAll() { m_LayerCheck = 0xffffffff; }
    void SetProjType(PROJ_TYPE _Type) { m_ProjType = _Type; }
    void SetWidth(float _Width)
    {
        m_Width = _Width;
        m_AspectRatio = m_Width / m_Height;
    }
    void SetHeight(float _Height)
    {
        m_Height = _Height;
        m_AspectRatio = m_Width / m_Height;
    }
    void SetFar(float _Far) { m_Far = _Far; }
    void SetFOV(float _FOV) { m_FOV = _FOV; }
    void SetProjScale(float _Scale) { m_ProjectionScale = _Scale; }

    int GetPriority() { return m_Priority; }
    bool GetLayerCheck(UINT _LayerIdx) { return m_LayerCheck & (1 << _LayerIdx); }
    PROJ_TYPE GetProjType() { return m_ProjType; }
    float GetWidth() { return m_Width; }
    float GetHeight() { return m_Height; }
    float GetAsepctRatio() { return m_AspectRatio; }
    float GetFar() { return m_Far; }
    float GetFOV() { return m_FOV; }
    float GetProjScale() { return m_ProjectionScale; }

private:
    void SortGameObject();

public:
    virtual void Begin() override;
    virtual void FinalTick() override;
    
    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Render();

public:
    CLONE(CCamera);
    CCamera();
    ~CCamera();
};

