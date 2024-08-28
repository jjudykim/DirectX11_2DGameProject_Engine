#pragma once
#include "CComponent.h"
class CTransform :
    public CComponent
{

private:
    Vec3     m_RelativePos;
    Vec3     m_RelativeScale;
    Vec3     m_RelativeRotation;

    Vec3     m_RelativeDir[3];
    Vec3     m_WorldDir[3];

    Matrix   m_matWorld;               // 이동, 크기, 회전
    bool     m_IndependentParent;       // 부모에 이동/크기/회전을 영향 받지 않도록 하는 On/Off

public:
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

    void Binding();

public:
    void SetRelativePos(Vec3 _Pos) { m_RelativePos = _Pos; }
    void SetRelativeScale(Vec3 _Scale) { m_RelativeScale = _Scale; }
    void SetRelativeRotation(Vec3 _Rot) { m_RelativeRotation = _Rot; }

    void SetRelativePos(float x, float y, float z) { m_RelativePos = Vec3(x, y, z); }
    void SetRelativeScale(float x, float y, float z) { m_RelativeScale = Vec3(x, y, z); }
    void SetRelativeRotation(float x, float y, float z) { m_RelativeRotation = Vec3(x, y, z); }

    void SetWorldMatrix(const Matrix& matWorld) { m_matWorld = matWorld; }

    void SetIndependentParent(bool _Set) { m_IndependentParent = _Set; }

    Vec3 GetRelativePos() { return m_RelativePos; }
    Vec3 GetWorldPos() { return m_matWorld.Translation(); }
    Vec3 GetRelativeScale() { return m_RelativeScale; }
    Vec3 GetWorldScale();

    Vec3 GetRelativeRotation() { return m_RelativeRotation; }
    Vec3 GetRelativeDir(DIR _Type) { return m_RelativeDir[_Type]; }
    Vec3 GetWorldDir(DIR _Type) { return m_WorldDir[_Type]; }

    Vec3 GetDir(DIR _Type) { return m_RelativeDir[_Type]; }
    const Matrix& GetWorldMat() { return m_matWorld; }

    bool IsIndependentParent() { return m_IndependentParent; }

public:
    CLONE(CTransform);
    CTransform();
    ~CTransform();
};

