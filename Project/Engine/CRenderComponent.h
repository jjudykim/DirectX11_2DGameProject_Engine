#pragma once
#include "CComponent.h"
class CRenderComponent :
    public CComponent
{
private:
    Ptr<CMesh>            m_Mesh;

    Ptr<CMaterial>        m_Mtrl;               // 현재 사용중인 재질
    Ptr<CMaterial>        m_SharedMtrl;         // 공유 재질 (master)
    Ptr<CMaterial>        m_DynamicMtrl;        // 임시 동적 재질

public:
    void SetMesh(Ptr<CMesh> _Mesh) { m_Mesh = _Mesh; }
    void SetMaterial(Ptr<CMaterial> _Mtrl);

    Ptr<CMesh> GetMesh() { return m_Mesh; }
    Ptr<CMaterial> GetMaterial() { return m_Mtrl; }
    Ptr<CMaterial> GetSharedMaterial();

    // Dynamic Material 생성 및 반환
    Ptr<CMaterial> GetDynamicMaterial();

public:
    virtual void FinalTick() = 0;
    virtual void Render() = 0;
    virtual CRenderComponent* Clone() = 0;

protected:
    void SaveDataToFile(FILE* _File);
    void LoadDataFromFile(FILE* _File);

public:
    CRenderComponent(COMPONENT_TYPE _Type);
    CRenderComponent(const CRenderComponent& _Origin);
    ~CRenderComponent();
};

