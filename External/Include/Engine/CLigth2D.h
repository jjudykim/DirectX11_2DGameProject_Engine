#pragma once
#include "CComponent.h"

class CLigth2D :
    public CComponent
{
private:
    tLightInfo m_Info;

public:
    void SetLightType(LIGHT_TYPE _Type);
    void SetRadius(float _Radius) { m_Info.Radius = _Radius; }
    void SetAngle(float _Angle) { m_Info.Angle = _Angle; }

    LIGHT_TYPE GetLightType() { return m_Info.Type; }
    float GetRadius() { return m_Info.Radius; }
    float GetAngle() { return m_Info.Angle; }

    const tLightInfo& GetLightInfo() { return m_Info; }

public:
    virtual void FinalTick() override;

public:
    CLONE(CLight2D);
    CLight2D();
    ~CLight2D();

};

