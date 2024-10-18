#pragma once
#include "ComponentUI.h"

class ParticleSystemUI :
    public ComponentUI
{
private:
    CParticleSystem* m_Particle;
    tParticleModule     m_Module;

    int                 m_ParticleCount;
    Ptr<CTexture>       m_ParticleTexture;

    UINT                m_UIHeight;

    virtual void Update() override;

private:
    void SetBasicOption();

    // Set Module Option
    void SetSpawn();
    void SetSpawnBurst();
    void SetVelocity();
    void SetScale();
    void SetDrag();
    void SetNoiseForce();
    void SetRender();

    void ChangeTexture(DWORD_PTR param);

public:
    ParticleSystemUI();
    ~ParticleSystemUI();
};

