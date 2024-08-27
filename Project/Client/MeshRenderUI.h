#pragma once
#include "ComponentUI.h"


class MeshRenderUI :
    public ComponentUI
{
private:
    CMeshRender*        m_MeshRender;
    TEX_PARAM           m_SelectTexParam;

    bool                m_UseSprite;
    bool                m_UseFlipBook;
    
    int                 m_ParamIdx;
    UINT                m_UIHeight;

private:
    virtual void Init() override;
    virtual void Update() override;

public:
    void SelectMesh(DWORD_PTR _AssetName);
    void SelectMaterial(DWORD_PTR _AssetName);
    void ShaderParameter();
    void ChangeTexture(DWORD_PTR Param);
    void ChangeSprite(DWORD_PTR Param);

public:
    MeshRenderUI();
    ~MeshRenderUI();
};

