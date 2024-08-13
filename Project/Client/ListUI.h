#pragma once
#include "EditorUI.h"


class ListUI :
    public EditorUI
{
private:
    vector<string>    m_vecList;
    int               m_SelectedIdx;
    string            m_SelectedName;
    bool              m_ShowNameOnly;

    EditorUI*         m_UI;
    DELEGATE_1        m_MemFunc;

public:
    void AddItem(const string& _item);
    void AddList(const vector<string>& _list);
    void AddDelegate(EditorUI* _UI, DELEGATE_1 _Func)
    {
        m_UI = _UI;
        m_MemFunc = _Func;
    }

public:
    void SetShowNameOnly(bool _Show) { m_ShowNameOnly = _Show; }
    string GetSelectName() { return m_SelectedName; }

public:
    virtual void Activate() override;
    virtual void Deactivate() override;

public:
    virtual void Update() override;
    

public:
    ListUI();
    ~ListUI();
};

