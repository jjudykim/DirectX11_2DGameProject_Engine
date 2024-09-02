#pragma once
#include "CComponent.h"

typedef void (*CALL_BACK)(void);
typedef void (CGameObject::* DELEGATE)(void);

class CRigidBody :
    public CComponent
{
private:
    Vec3    m_VelocityByGravity;    // 중력에 의해서 증가하는 속도

    float   m_GravityAccel;         // 중력 가속도
    float   m_MaxGravitySpeed;      // 중력으로 발생한 낙하속도 제한

    bool    m_UseGravity;
    bool    m_Ground;               // 땅 위에 서있는지 체크
    float   m_JumpSpeed;            // 점프 속력


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

public:
    Vec3 GetGravityVelocity() { return m_VelocityByGravity; }


    void SetJumpSpeed(float _Speed) { m_JumpSpeed = _Speed; }
    void SetGravityVelocity(Vec3 _Velocity) { m_VelocityByGravity = _Velocity; }
    void SetGroundFunc(void (*_pFunc)(void)) { m_GroundFunc = _pFunc; }
    void SetAirFunc(void(*_pFunc)(void)) { m_AirFunc = _pFunc; }

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
                (m_GroundInst->*m_GroundDelegate)();
        }
        else
        {
            if (nullptr != m_AirFunc)
                m_AirFunc();

            if (m_AirInst && m_AirDelegate)
                (m_AirInst->*m_AirDelegate)();
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

