#pragma once
#include "ComponentUI.h"

class RigidBodyUI :
    public ComponentUI
{
private:
    class CRigidBody*   m_RigidBody;

    float               m_Velocity[3];
    float               m_VelocityByGravity[3];

    float               m_Mass;
                        
    float               m_InitWalkSpeed;
    float               m_MaxWalkSpeed;
                        
    float               m_Friction;
                        
    float               m_GravityAccel;
    float               m_MaxGravitySpeed;
                        
    float               m_JumpSpeed;
    bool                m_UseGravity;
    bool                m_Ground;

public:
    virtual void Init() override;
    virtual void Update() override;

    void GetRigidBodyData(CRigidBody* _RigidBody);
    void SetRigidBodyData(CRigidBody* _RigidBody);

public:
    RigidBodyUI();
    ~RigidBodyUI();
  
};

