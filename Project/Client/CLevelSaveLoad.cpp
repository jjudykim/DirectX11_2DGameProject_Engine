#include "pch.h"
#include "CLevelSaveLoad.h"

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Scripts/CScriptMgr.h>

void CLevelSaveLoad::SaveLevel(const wstring& _FilePath, CLevel* _Level)
{
	assert(_Level && _Level->GetState() == LEVEL_STATE::STOP);

	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	// Level Name
	SaveWString(_Level->GetName(), File);

	// Layer
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		// Layer Name
		CLayer* pLayer = _Level->GetLayer(i);
		SaveWString(pLayer->GetName(), File);

		// GameObject - Parents Count 
		const vector<CGameObject*>& vecParents = pLayer->GetParentObjects();

		size_t count = vecParents.size();
		fwrite(&count, sizeof(size_t), 1, File);

		// GameObject - Save
		for (size_t i = 0; i < vecParents.size(); ++i)
		{
			SaveGameObject(File, vecParents[i]);
		}
	}

	fclose(File);
}

CLevel* CLevelSaveLoad::LoadLevel(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	// Create Level
	CLevel* pNewLevel = new CLevel;

	// Level Name
	wstring LevelName;
	LoadWString(LevelName, File);
	pNewLevel->SetName(LevelName);

	// Layer Info
	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		CLayer* pLayer = pNewLevel->GetLayer(i);

		// Layer Name
		wstring LayerName;
		LoadWString(LayerName, File);
		pLayer->SetName(LayerName);

		// GameObject - Parents Count
		size_t count = 0;
		fread(&count, sizeof(size_t), 1, File);

		// GameObject - Load
		for (size_t i = 0; i < count; ++i)
		{
			CGameObject* pLoadedObject = LoadGameObject(File);
			pLayer->AddObject(pLoadedObject, false);
		}
	}
	fclose(File);

	return pNewLevel;
}

void CLevelSaveLoad::SaveGameObject(FILE* _File, CGameObject* _Object)
{
	// GameObject's Name
	SaveWString(_Object->GetName(), _File);

	// Component Info
	UINT i = 0;
	for (; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		CComponent* pComponent = _Object->GetComponent((COMPONENT_TYPE)i);

		if (pComponent == nullptr)
			continue;

		// Component Type
		COMPONENT_TYPE Type = pComponent->GetComponentType();
		fwrite(&Type, sizeof(COMPONENT_TYPE), 1, _File);

		pComponent->SaveToFile(_File);
	}

	// COMPONENT_TYPE::END
	fwrite(&i, sizeof(COMPONENT_TYPE), 1, _File);

	// Script Info
	const vector<CScript*> vecScripts = _Object->GetScripts();

	// Script count
	size_t ScriptCount = vecScripts.size();
	fwrite(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script Name
		wstring ScriptName = CScriptMgr::GetScriptName(vecScripts[i]);
		SaveWString(ScriptName, _File);

		// Script - Save
		vecScripts[i]->SaveToFile(_File);
	}

	// Child Info
	const vector<CGameObject*>& vecChild = _Object->GetChildren();
	size_t ChildCount = vecChild.size();
	fwrite(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < vecChild.size(); ++i)
	{
		SaveGameObject(_File, vecChild[i]);
	}
}

CGameObject* CLevelSaveLoad::LoadGameObject(FILE* _File)
{
	CGameObject* pObject = new CGameObject;

	// Name
	wstring Name;
	LoadWString(Name, _File);
	pObject->SetName(Name);

	// Component Info
	COMPONENT_TYPE Type = COMPONENT_TYPE::END;
	while (true)
	{
		// Component Type
		fread(&Type, sizeof(COMPONENT_TYPE), 1, _File);
	
		if (COMPONENT_TYPE::END == Type)
			break;

		// Component Data 
		CComponent* pComponent = GetComponent(Type);
		pObject->AddComponent(pComponent);
		pComponent->LoadFromFile(_File);
	}

	// Script Info
	size_t ScriptCount = 0;
	fread(&ScriptCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ScriptCount; ++i)
	{
		// Script Name
		wstring ScriptName;
		LoadWString(ScriptName, _File);

		// Script Create
		CScript* pScript = CScriptMgr::GetScript(ScriptName);
		pScript->LoadFromFile(_File);
		pObject->AddComponent(pScript);
	}


	// Child Info
	size_t ChildCount = 0;
	fread(&ChildCount, sizeof(size_t), 1, _File);

	for (size_t i = 0; i < ChildCount; ++i)
	{
		CGameObject* pChildObject = LoadGameObject(_File);
		pObject->AddChild(pChildObject);
	}

	return pObject;
}

CComponent* CLevelSaveLoad::GetComponent(COMPONENT_TYPE _Type)
{
	switch (_Type)
	{
	case COMPONENT_TYPE::TRANSFORM:
		return new CTransform;

	case COMPONENT_TYPE::COLLIDER2D:
		return new CCollider2D;

	case COMPONENT_TYPE::COLLIDER3D:

		break;
	case COMPONENT_TYPE::LIGHT2D:
		return  new CLight2D;

	case COMPONENT_TYPE::LIGHT3D:
		break;

	case COMPONENT_TYPE::FLIPBOOKCOMPONENT:
		return new CFlipBookComponent;

	case COMPONENT_TYPE::ANIMATOR3D:
		break;

	case COMPONENT_TYPE::FSM:
		return new CFSM;
		break;

	case COMPONENT_TYPE::RIGIDBODY:
		break;

	case COMPONENT_TYPE::CAMERA:
		return  new CCamera;

	case COMPONENT_TYPE::MESHRENDER:
		return  new CMeshRender;

	case COMPONENT_TYPE::TILEMAP:
		return  new CTileMap;

	case COMPONENT_TYPE::PARTICLE_SYSTEM:
		break;

	case COMPONENT_TYPE::DECAL:
		break;

	case COMPONENT_TYPE::SKYBOX:
		break;

	case COMPONENT_TYPE::LANDSCAPE:
		break;
	}

	return nullptr;
}
