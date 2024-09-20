#pragma once
#include "CComponent.h"

class CCollider2D :
    public CComponent
{
private:
    Vec3      m_Offset;
    Vec3      m_Scale;                 // 배율 or 절대적인 크기
    Matrix    m_matColWorld;           // 충돌체의 최종 월드 상태

    int       m_OverlapCount;
    bool      m_IndependentScale;      // 오브젝트의 크기에 영향을 받을지에 대한 여부

public:
    void SetOffset(Vec3 _Offset) { m_Offset = _Offset; }
    void SetScale(Vec3 _Scale) { m_Scale = _Scale; }
    void SetIndependentScale(bool _Set) { m_IndependentScale = _Set;  }

    Vec3 GetOffset() { return m_Offset; }
    Vec3 GetScale() { return m_Scale; }
    Vec3 GetWorldPos() { return m_matColWorld.Translation(); }
    bool GetIndependentScale() { return m_IndependentScale; }
    const Matrix& GetWorldMat() { return m_matColWorld; }

    int GetOverlapCount() { return m_OverlapCount; }
    void AddOverlapCount() { m_OverlapCount++; }
    void ResetOverlapCount() { m_OverlapCount = 0; }

public:
    virtual void BeginOverlap(CCollider2D* _Other);
    virtual void Overlap(CCollider2D* _Other);
    virtual void EndOverlap(CCollider2D* _Other);

public:
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CCollider2D);
    CCollider2D();
    CCollider2D(const CCollider2D& _Origin);
    ~CCollider2D();
};

