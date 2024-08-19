#include "pch.h"
#include "TreeUI.h"

// ===================
//      TreeNode
// ===================

TreeNode::TreeNode(UINT _ID)
	: m_Owner(nullptr)
	, m_ParentNode(nullptr)
	, m_ID(_ID)
	, m_Data(0)
	, m_Frame(false)
	, m_Category(false)
	, m_Selected(false)
{
}

TreeNode::~TreeNode()
{
	Delete_Vec(m_vecChildNode);
}

void TreeNode::Update()
{
	UINT Flag = ImGuiTreeNodeFlags_OpenOnDoubleClick
		      | ImGuiTreeNodeFlags_OpenOnArrow
		      | ImGuiTreeNodeFlags_SpanAvailWidth;

	if (m_Frame)
		Flag |= ImGuiTreeNodeFlags_Framed;

	if (m_Selected)
		Flag |= ImGuiTreeNodeFlags_Selected;

	char Name[255] = {};

	if (m_vecChildNode.empty())
	{
		Flag |= ImGuiTreeNodeFlags_Leaf;
		
		if (m_Frame)
			sprintf_s(Name, 255, "   %s##%d", m_Name.c_str(), m_ID);
		else
			sprintf_s(Name, 255, "%s##%d", m_Name.c_str(), m_ID);
	}
	else
	{
		sprintf_s(Name, 255, "%s##%d", m_Name.c_str(), m_ID);
	}

	string strName = Name;

	// NameOnly
	if (m_Owner->IsShowNameOnly())
	{
		path Path = Name;
		strName = Path.stem().string();
	}


	if (ImGui::TreeNodeEx(strName.c_str(), Flag))
	{
		// Click Check
		if (ImGui::IsItemHovered() &&  ImGui::IsMouseReleased(ImGuiMouseButton_Left))
		{
			m_Owner->SetSelectedNode(this);
		}

		// Drag Check
		DragCheck();

		// Drop Check
		DropCheck();

		// Right Click Popup Menu
		if (ImGui::BeginPopupContextItem())
		{
			m_Owner->PopupMenu(this);

			m_Owner->SetSelectedNode(this);
		}

		for (size_t i = 0; i < m_vecChildNode.size(); ++i)
		{
			m_vecChildNode[i]->Update();
		}

		ImGui::TreePop();
	}
}

void TreeNode::DragCheck()
{
	if (m_Owner->IsDrag())
	{
		if (ImGui::BeginDragDropSource())
		{
			TreeNode* pThis = this;

			ImGui::SetDragDropPayload(m_Owner->GetName().c_str(), &pThis, sizeof(TreeNode*));
			ImGui::Text(m_Name.c_str());
			ImGui::EndDragDropSource();

			m_Owner->SetDraggedNode(this);
		}
	}
}

void TreeNode::DropCheck()
{
	if (!m_Owner->IsDrop())
		return;

	if (ImGui::BeginDragDropTarget())
	{
		m_Owner->SetDroppedNode(this);
		ImGui::EndDragDropTarget();
	}
}



// ===================
//       TreeUI
// ===================

TreeUI::TreeUI()
	: m_Root(nullptr)
	, m_SelectedNode(nullptr)
	, m_DraggedNode(nullptr)
	, m_DroppedNode(nullptr)
	, m_NodeID(0)
	, m_ShowRoot(false)
	, m_ShowNameOnly(false)
	, m_UseDrag(false)
	, m_UseDrop(false)
	, m_ClickedInst(nullptr)
	, m_ClickedFunc(nullptr)
{
}

TreeUI::~TreeUI()
{
	Clear();
}

void TreeUI::Update()
{
	if (m_Root == nullptr)
		return;

	if (m_ShowRoot)
	{
		m_Root->Update();
	}
	else
	{
		for (size_t i = 0; i < m_Root->m_vecChildNode.size(); ++i)
		{
			m_Root->m_vecChildNode[i]->Update();
		}
	}

	if (ImGui::IsMouseReleased(ImGuiMouseButton_Left))
	{
		if (m_DraggedNode && !m_DroppedNode)
		{
			if (m_SelfDragDropInst && m_SelfDragDropFunc)
			{
				(m_SelfDragDropInst->*m_SelfDragDropFunc)((DWORD_PTR)m_DraggedNode, 0);
			}
		}
		m_DroppedNode = m_DraggedNode = nullptr;
	}
}

TreeNode* TreeUI::AddNode(TreeNode* _Parent, const string& _Name, DWORD_PTR _Data)
{
	// Node 생성, 이름 및 ID 세팅
	TreeNode* pNode = new TreeNode(m_NodeID++);
	pNode->m_Owner = this;
	pNode->SetName(_Name);
	pNode->m_Data = _Data;

	if (_Parent == nullptr)
	{
		assert(!m_Root);

		m_Root = pNode;
	}
	else
	{
		_Parent->AddChildNode(pNode);
	}

	return pNode;
}

TreeNode* TreeUI::AddNodeByDir(TreeNode* _Parent, const string& _Name, map<string, TreeNode*>& _mapDirNode, Ptr<CAsset> _Data)
{
	TreeNode* pNewNode = new TreeNode(m_NodeID++);
	pNewNode->m_Owner = this;
	pNewNode->SetName(_Name);
	pNewNode->m_Data = (DWORD_PTR)_Data.Get();

	// Directory Name의 Node 생성 / 찾기
	Ptr<CSprite> spr = dynamic_cast<CSprite*>(_Data.Get());
	wstring name = spr->GetBundleName();

	if (spr != nullptr)
	{
		TreeNode* pDirNode;
		string strBundle = string(spr->GetBundleName().begin(), spr->GetBundleName().end());
		
		auto dirNode = _mapDirNode.find(strBundle);
		if (dirNode != _mapDirNode.end())
		{
			pDirNode = dirNode->second;
			pDirNode->AddChildNode(pNewNode);
		}
		else
		{
			pDirNode = new TreeNode(m_NodeID++);
			pDirNode->m_Owner = this;
			pDirNode->SetName(strBundle);
			pDirNode->SetCategory(true);
			pDirNode->m_Data = (DWORD_PTR)strBundle.c_str();
			_Parent->AddChildNode(pDirNode);

			_mapDirNode.insert(make_pair(strBundle, pDirNode));
			pDirNode->AddChildNode(pNewNode);
		}
	}

	return pNewNode;
}

void TreeUI::SetSelectedNode(TreeNode* _Node)
{
	// 기존의 Selected Node -> 해제
	if (m_SelectedNode != nullptr)
		m_SelectedNode->m_Selected = false;

	// New Node를 Selected Node로 설정
	m_SelectedNode = _Node;

	if (m_SelectedNode != nullptr)
	{
		m_SelectedNode->m_Selected = true;

		if (m_ClickedInst && m_ClickedFunc)
		{
			(m_ClickedInst->*m_ClickedFunc)((DWORD_PTR)m_SelectedNode);
		}
	}
}

void TreeUI::SetDraggedNode(TreeNode* _Node)
{
	m_DraggedNode = _Node;
}

void TreeUI::SetDroppedNode(TreeNode* _Node)
{
	// Drag된 노드가 없는 경우 (외부 데이터가 Tree로 Drop)
	if (m_DraggedNode == nullptr)
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(m_DropPayLoadName.c_str());
		if (payload)
		{
			m_DroppedNode = _Node;

			if (m_DropInst && m_DropFunc)
				(m_DropInst->*m_DropFunc)((DWORD_PTR)payload->Data, (DWORD_PTR)m_DroppedNode);
		}
	}

	// Self Drag Drop된 경우
	else
	{
		assert(m_DraggedNode->m_Owner == this);

		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload(GetName().c_str());
		if (payload)
		{
			m_DroppedNode = _Node;

			if (m_SelfDragDropInst && m_SelfDragDropFunc)
				(m_SelfDragDropInst->*m_SelfDragDropFunc)((DWORD_PTR)m_DraggedNode, (DWORD_PTR)m_DroppedNode);
		}
	}
}

void TreeUI::PopupMenu(TreeNode* _Node)
{
	if (m_PopupInst && m_PopupFunc)
	{
		(m_PopupInst->*m_PopupFunc)((DWORD_PTR)_Node);
	}
}


void TreeUI::Clear()
{
	if (m_Root != nullptr)
	{
		delete m_Root;
		m_Root = nullptr;
	}
}
