#include "pch.h"
#include "Outliner.h"

#include "TreeUI.h"
#include "Inspector.h"
#include "CEditorMgr.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>

Outliner::Outliner()
{
	m_Tree = new TreeUI;
	m_Tree->SetName("OutlinerTree");
	AddChild(m_Tree);

	// Tree Option Setting
	m_Tree->ShowRoot(false);

	// Drag / Drop On
	m_Tree->UseDrag(true);
	m_Tree->UseDrop(true);

	// Clicked Delegate 등록
	m_Tree->AddClickedDelegate(this, (DELEGATE_1)&Outliner::GameObjectClicked);

	// Self Drag/Drop Delegate 등록
	m_Tree->AddDragDropDelegate(this, (DELEGATE_2)&Outliner::GameObjectAddChild);

	// 외부 Node Drop Delegate 등록
	m_Tree->AddDropDelegate(this, (DELEGATE_2)&Outliner::DroppedFromOuter);

	// Popup Delegate 등록
	m_Tree->AddPopupDelegate(this, (DELEGATE_1)&Outliner::PopupMenu);         

	m_Tree->SetDropPayLoadName("ContentTree");

	// Asset 상태를 Content의 TreeUI에 반영
	RenewLevel();
}

Outliner::~Outliner()
{
}

void Outliner::Update()
{
	if (CLevelMgr::GetInst()->IsLevelChanged())
		RenewLevel();
}

void Outliner::ObjectViewer(CLevel* _Level, class TreeNode* _RootNode)
{
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = _Level->GetLayer(i);
		const vector<CGameObject*>& vecObjects = pLayer->GetParentObjects();

		for (size_t i = 0; i < vecObjects.size(); ++i)
		{
			AddGameObject(_RootNode, vecObjects[i]);
		}
	}
}

void Outliner::RenewLevel()
{
	// 모든 Tree 내용 삭제
	m_Tree->Clear();

	// RootNode 생성
	TreeNode* pRootNode = m_Tree->AddNode(nullptr, "Root", 0);

	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	if (pLevel == nullptr)
		return;

	ObjectViewer(pLevel, pRootNode);
}

void Outliner::AddGameObject(TreeNode* pNode, CGameObject* _Object)
{
	string ObjectName = string(_Object->GetName().begin(), _Object->GetName().end());
	TreeNode* pObjectNode = m_Tree->AddNode(pNode, ObjectName.c_str(), (DWORD_PTR)_Object);
	
	const vector<CGameObject*>& vecChild = _Object->GetChildren();

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		AddGameObject(pObjectNode, vecChild[i]);
	}
}

void Outliner::PopupMenu(DWORD_PTR _Param)
{
	TreeNode* pTargetNode = (TreeNode*)_Param;
	CGameObject* pObject = (CGameObject*)pTargetNode->GetData();

	if (ImGui::MenuItem("Make To Prefab"))
	{
		CGameObject* CloneObj = pObject->Clone();
		 
		Ptr<CPrefab> pPrefab = new CPrefab;
		
		pPrefab->SetProtoObject(CloneObj);
		wstring PrefabName = CloneObj->GetName() + L"Pref";
		pPrefab->SetName(PrefabName);

		CAssetMgr::GetInst()->AddAsset(PrefabName, pPrefab);

		wstring PrefPath = CPathMgr::GetInst()->GetContentPath() + L"prefab\\" + PrefabName + L".pref";
		pPrefab->Save(PrefPath);
	}

	if (ImGui::MenuItem("Clone"))
	{
		CGameObject* CloneObj = pObject->Clone();

		wstring NewName = pObject->GetName() + L"C";
		CloneObj->SetName(NewName);

		CreateObject(CloneObj, 0);

		ImGui::CloseCurrentPopup();
	}

	if (ImGui::MenuItem("Delete"))
	{
		DeleteObject(pObject);
		Inspector* inspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
		vector<EditorUI*> ChildUI = inspector->GetChildren();
		for (size_t i = 0; i < ChildUI.size(); ++i)
		{
			ChildUI[i]->SetActive(false);
		}
		inspector->SetTargetObject(nullptr);
		ImGui::CloseCurrentPopup();
	}

	if (ImGui::BeginMenu("Delete Component"))
	{
		if (ImGui::MenuItem("1. Transform"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::TRANSFORM) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::TRANSFORM);
			
			ImGui::CloseCurrentPopup();
		}	
		if (ImGui::MenuItem("2. Collider2D"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::COLLIDER2D) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::COLLIDER2D);

			ImGui::CloseCurrentPopup();
		}
		
		if (ImGui::MenuItem("3. FlipBook"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT);

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("4. FSM"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::FSM) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::FSM);

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("5. RigidBody"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::RIGIDBODY) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::RIGIDBODY);

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("6. Camera"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::CAMERA) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::CAMERA);

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("7. Light2D"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::LIGHT2D) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::LIGHT2D);

			ImGui::CloseCurrentPopup();
		}

		ImGui::SeparatorText("Render Component");

		if (ImGui::MenuItem("MeshRender"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::MESHRENDER) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::MESHRENDER);

			ImGui::CloseCurrentPopup();
		}

		if (ImGui::MenuItem("TileMap"))
		{
			if (pObject->GetComponent(COMPONENT_TYPE::TILEMAP) != nullptr)
				pObject->DeleteComponent(COMPONENT_TYPE::TILEMAP);

			ImGui::CloseCurrentPopup();
		}

		Inspector* inspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
		inspector->SetTargetObject(pObject);

		ImGui::EndMenu();
	}

	ImGui::EndPopup();
}

void Outliner::GameObjectAddChild(DWORD_PTR _Param1, DWORD_PTR _Param2)
{
	TreeNode* pDragNode = (TreeNode*)_Param1;
	TreeNode* pDropNode = (TreeNode*)_Param2;

	CGameObject* pDragObject = (CGameObject*)pDragNode->GetData();
	CGameObject* pDropObject = nullptr;

	// Drag Object를 Drop Object의 자식으로 넣어줌
	if (pDropNode)
	{
		pDropObject = (CGameObject*)pDropNode->GetData();

		if (pDropObject->IsAncestor(pDragObject))
			return;

		pDropObject->AddChild(pDragObject);
	}
	// Drop 목적지 X -> Drag Object를 최상위 부모로 만들어준다
	else
	{
		if (!pDragObject->GetParent())
			return;

		pDragObject->DeregisterChild();

		CLevelMgr::GetInst()->GetCurrentLevel()->RegisterAsParent(pDragObject->GetLayerIdx(), pDragObject);
	}
}

void Outliner::DroppedFromOuter(DWORD_PTR _OuterData, DWORD_PTR _DropNode)
{
	TreeNode* ContentNode = *((TreeNode**)_OuterData);
	TreeNode* DropNode = (TreeNode*)_DropNode;
}

void Outliner::GameObjectClicked(DWORD_PTR _Param)
{
	TreeNode* pNode = (TreeNode*)_Param;
	CGameObject* pObject = (CGameObject*)pNode->GetData();

	Inspector* pInspector = (Inspector*)CEditorMgr::GetInst()->FindEditorUI("Inspector");
	pInspector->SetTargetObject(pObject);
	//ImGui::SetWindowFocus(nullptr);
}