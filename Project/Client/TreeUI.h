#pragma once
#include "EditorUI.h"

class TreeNode
{
private:
    class TreeUI*        m_Owner;

    const UINT           m_ID;
    string               m_Name;
    DWORD_PTR            m_Data;
    TreeNode*            m_ParentNode;
    vector<TreeNode*>    m_vecChildNode;

    bool                 m_Frame;
    bool                 m_Selected;

public:
    void SetName(const string& _Name) { m_Name = _Name; }
    void SetFrame(bool _Frame) { m_Frame = _Frame; }

    const string& GetName() { return m_Name; }
    bool IsFrame() { return m_Frame; }
    DWORD_PTR GetData() { return m_Data; }

public:
    void AddChildNode(TreeNode* _Node) { m_vecChildNode.push_back(_Node); }
    void DragCheck();
    void DropCheck();

public:
    void Update();

public:
    TreeNode(UINT _ID);
    ~TreeNode();

    friend class TreeUI;
};


class TreeUI :
    public EditorUI
{
private:
    TreeNode* m_Root;
    TreeNode* m_SelectedNode;
    TreeNode* m_DraggedNode;
    TreeNode* m_DroppedNode;

    UINT          m_NodeID;
    bool          m_ShowRoot;
    bool          m_ShowNameOnly;
    bool          m_UseDrag;
    bool          m_UseDrop;

    EditorUI*     m_ClickedInst;
    DELEGATE_1    m_ClickedFunc;

    EditorUI*     m_SelfDragDropInst;
    DELEGATE_2    m_SelfDragDropFunc;

    EditorUI*     m_DropInst;
    DELEGATE_2    m_DropFunc;
    string        m_DropPayLoadName;

    EditorUI*     m_PopupInst;
    DELEGATE_1    m_PopupFunc;

public:
    TreeNode* AddNode(TreeNode* _Parent, const string& _Name, DWORD_PTR _Data = 0);
    void SetSelectedNode(TreeNode* _Node);
    void Clear();

public:
    void ShowRoot(bool _Show) { m_ShowRoot = _Show; }
    void UseDrag(bool _Drag) { m_UseDrag = _Drag; }
    void UseDrop(bool _Drop) { m_UseDrop = _Drop; }
    void SetDraggedNode(TreeNode* _Node);
    void SetDroppedNode(TreeNode* _Node);
    void ShowNameOnly(bool _NameOnly) { m_ShowNameOnly = _NameOnly; }

    bool IsShowRoot() { return m_ShowRoot; }
    bool IsDrag() { return m_UseDrag; }
    bool IsDrop() { return m_UseDrop; }
    bool IsShowNameOnly() { return m_ShowNameOnly; }
    void PopupMenu(TreeNode* _Node);

    void SetDropPayLoadName(const string& _Name) { m_DropPayLoadName = _Name; }
    const string GetDropPayLoadName() { return m_DropPayLoadName; }

    void AddClickedDelegate(EditorUI* _Inst, DELEGATE_1 _Func) { m_ClickedInst = _Inst, m_ClickedFunc = _Func; }
    void AddDragDropDelegate(EditorUI* _Inst, DELEGATE_2 _Func) { m_SelfDragDropInst = _Inst, m_SelfDragDropFunc = _Func; }
    void AddDropDelegate(EditorUI* _Inst, DELEGATE_2 _Func) { m_DropInst = _Inst, m_DropFunc = _Func; }
    void AddPopupDelegate(EditorUI* _Inst, DELEGATE_1 _Func) { m_PopupInst = _Inst, m_PopupFunc = _Func; }

public:
    virtual void Update() override;

public:
    TreeUI();
    ~TreeUI();
};

