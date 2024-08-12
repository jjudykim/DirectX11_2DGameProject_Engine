#pragma once
#include "ComponentUI.h"

class ScriptUI :
    public ComponentUI
{
private:
    class CScript*     m_Script;
                       
    UINT               m_UIHeight;

    Ptr<CPrefab>*      m_SelectedPrefab;

private:
    void SelectPrefab(DWORD_PTR _ListUI);

public:
    void SetTargetScript(CScript* _Script);
    CScript* GetTargetScript() { return m_Script; }

public:
    virtual void Update() override;

public:
    ScriptUI();
    ~ScriptUI();
};

