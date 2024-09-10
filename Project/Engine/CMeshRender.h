#pragma once
#include "CRenderComponent.h"

class CMeshRender :
    public CRenderComponent
{
private:
    bool        m_UseSpriteAsTex;
    bool        m_UseBlinkEffect;
    int         m_ParamIdx;

public:
    virtual void FinalTick() override;
    virtual void Render() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    void SetUseSpriteAsTex(bool _Use) { m_UseSpriteAsTex = _Use; }
    void SetParamIdx(int _index) { m_ParamIdx = _index; }

    bool GetUseSpriteAsTex() { return m_UseSpriteAsTex; }
    int GetParamIndex() { return m_ParamIdx; }

public:
    CLONE(CMeshRender);
    CMeshRender();
    ~CMeshRender();
};

