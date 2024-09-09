#pragma once
#include "CComponent.h"

typedef void (*CALL_BACK)(void);
typedef void (CScript::* DELEGATE)(void);

class CRigidBody :
    public CComponent
{
private:
    Vec3    m_Velocity;             // 속도
    Vec3    m_VelocityByGravity;    // 중력에 의해서 증가하는 속도
    Vec3    m_AddVelocity;          // 최종 속도에 누적될 속도

    float   m_Mass;                 // 질량
    Vec3    m_Force;                // 힘

    float   m_InitWalkSpeed;        // 최소 보장 속력
    float   m_MaxWalkSpeed;         // 최대 제한 속력

    float   m_Friction;             // 마찰력

    float   m_GravityAccel;         // 중력 가속도
    float   m_MaxGravitySpeed;      // 중력으로 발생한 낙하속도 제한

    float   m_JumpSpeed;            // 점프 속력
    bool    m_UseGravity;
    bool    m_Ground;               
    

    // Ground On / Off 호출시킬 함수포인터
    // CallBack
    CALL_BACK    m_GroundFunc;
    CALL_BACK    m_AirFunc;

    // Delegate
    CGameObject* m_GroundInst;
    DELEGATE     m_GroundDelegate;

    CGameObject* m_AirInst;
    DELEGATE     m_AirDelegate;

public:
    void Jump();
    void CurvedJump(bool _Replay, UNITVEC_TYPE _Dir, float _Time);

public:
    void SetVelocity(Vec3 _Velocity) { m_Velocity = _Velocity; }
    void SetVelocityByGravity(Vec3 _Velocity) { m_VelocityByGravity = _Velocity; }
    void AddForce(Vec3 _vForce) { m_Force += _vForce; }
    void SetMass(float _Mass) { m_Mass = _Mass; }
    void SetInitialWalkSpeed(float _Speed) { m_InitWalkSpeed = _Speed; }
    void SetMaxWalkSpeed(float _Speed) { m_MaxWalkSpeed = _Speed; }
    void SetMaxGravitySpeed(float _Speed) { m_MaxGravitySpeed = _Speed; }
    void SetFriction(float _Friction) { m_Friction = _Friction; }
    void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }
    void SetGravityAccel(float _Accel) { m_GravityAccel = _Accel; }
    void SetUseGravity(bool _Use) { m_UseGravity = _Use; }
    
    void SetGroundFunc(void (*_pFunc)(void)) { m_GroundFunc = _pFunc; }
    void SetAirFunc(void(*_pFunc)(void)) { m_AirFunc = _pFunc; }

public:
    Vec3 GetVelocity() { return m_Velocity; }
    Vec3 GetVelocityByGravity() { return m_VelocityByGravity; }
    float GetMass() { return m_Mass; }
    float GetInitialWalkSpeed() { return m_InitWalkSpeed; }
    float GetMaxWalkSpeed() { return m_MaxWalkSpeed; }
    float GetMaxGravitySpeed() { return m_MaxGravitySpeed; }
    float GetFriction() { return m_Friction; }
    float GetJumpSpeed() { return m_JumpSpeed; }
    float GetGravityAccel() { return m_GravityAccel; }
    bool IsUseGravity() { return m_UseGravity; }
    bool IsGround() { return m_Ground; }

    void SetGroundDelegate(CGameObject* _Inst, DELEGATE _MemFunc)
    {
        m_GroundInst = _Inst;
        m_GroundDelegate = _MemFunc;
    }

    void SetAirDelegate(CGameObject* _Inst, DELEGATE _MemFunc)
    {
        m_AirInst = _Inst;
        m_AirDelegate = _MemFunc;
    }

    void UseGravity(bool _Use)
    {
        m_UseGravity = _Use;
        if (!m_UseGravity)
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);
    }

    void SetGround(bool _Ground)
    {
        if (m_Ground == _Ground)
            return;

        m_Ground = _Ground;

        if (m_Ground)
        {
            m_VelocityByGravity = Vec3(0.f, 0.f, 0.f);

            if (nullptr != m_GroundFunc)
                m_GroundFunc();

            if (m_GroundInst && m_GroundDelegate)
                (m_GroundInst->GetScriptByIndex(0)->*m_GroundDelegate)();
        }
        else
        {
            if (nullptr != m_AirFunc)
                m_AirFunc();

            if (m_AirInst && m_AirDelegate)
                (m_AirInst->GetScriptByIndex(0)->*m_AirDelegate)();
        }
    }


public:
    virtual void FinalTick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    CLONE(CRigidBody);
    CRigidBody();
    CRigidBody(const CRigidBody& _Other);
    ~CRigidBody();
};

