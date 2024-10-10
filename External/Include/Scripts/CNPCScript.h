#pragma once
#include "Engine\CScript.h"

class CNPCScript :
    public CScript
{
private:
    int             m_NPCType;           // NPCType : Marshmellow(1), Frank(2), Gates(3), Jobs(4)
    Vec3            m_vTalkBtn;
                    
    bool            m_IsAbleToTalk;
                    
    CGameObject*    m_TalkBtn;

public:
    virtual void Begin() override;
    virtual void Tick() override;

    virtual void SaveToFile(FILE* _File) override;
    virtual void LoadFromFile(FILE* _File) override;

public:
    virtual void BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;
    virtual void EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider) override;

public:
    CLONE(CNPCScript);
    CNPCScript();
    ~CNPCScript();

};

