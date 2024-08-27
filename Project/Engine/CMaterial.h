#pragma once
#include "CAsset.h"

#include "Ptr.h"
#include "CTexture.h"
#include "CSprite.h"

class CMaterial :
    public CAsset
{
private:
    Ptr<CGraphicShader>     m_Shader;

    tMtrlConst              m_Const;
    Ptr<CTexture>           m_arrTex[TEX_PARAM::END];

    bool                    m_UseSpriteAsTex;                 // Sprite�� Texture�ν� ��������� ���� ����
    Ptr<CSprite>            m_arrSprite[TEX_PARAM::END];

public:
    template<typename T>
    void SetScalarParam(SCALAR_PARAM _Param, const T& Data);
    void SetTexParam(TEX_PARAM _Param, Ptr<CTexture> _Tex) { m_arrTex[_Param] = _Tex; }
    void SetUseSpriteAsTex(bool _Use) { m_UseSpriteAsTex = _Use; }
    void SetSprite(TEX_PARAM _Param, Ptr<CSprite> _Sprite) { m_arrSprite[_Param] = _Sprite; }

    void* GetScalarParam(SCALAR_PARAM _Param);
    Ptr<CTexture> GetTexParam(TEX_PARAM _Param) { return m_arrTex[(UINT)_Param]; }
    bool GetUseSpriteAsTex() { return m_UseSpriteAsTex; }
    Ptr<CSprite> GetSprite(TEX_PARAM _Param) { return m_arrSprite[(UINT)_Param]; }

public:
    void SetShader(Ptr<CGraphicShader> _Shader) { m_Shader = _Shader; }
    Ptr<CGraphicShader> GetShader() { return m_Shader; }

public:
    void Binding();
    void Binding(int _index);

public:
    virtual int Load(const wstring& _FilePath) override;
    virtual int Save(const wstring& _RelativePath) override;

public:
    CLONE(CMaterial);
    CMaterial(bool _IsEngine = false);
    ~CMaterial();
};

template<typename T>
void CMaterial::SetScalarParam(SCALAR_PARAM _Param, const T& _Data)
{
    if constexpr (std::is_same_v<T, int>) { m_Const.iArr[_Param] = _Data; }
    if constexpr (std::is_same_v<T, float>) { m_Const.fArr[_Param - FLOAT_0] = _Data; }
    if constexpr (std::is_same_v<T, Vec2>) { m_Const.v2Arr[_Param - VEC2_0] = _Data; }
    if constexpr (std::is_same_v<T, Vec4>) { m_Const.v4Arr[_Param - VEC4_0] = _Data; }
    if constexpr (std::is_same_v<T, Matrix>) { m_Const.matArr[_Param - MAT_0] = _Data; }
}

