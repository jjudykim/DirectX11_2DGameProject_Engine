#include "pch.h"

#include "CRenderMgr.h"
#include "CTaskMgr.h"

#include "CGameObject.h"
#include "func.h"

void CreateObject(CGameObject* _NewObject, int _LayerIndex)
{
	tTask task = {};
	task.Type = TASK_TYPE::CREATE_OBJECT;
	task.Param_0 = _LayerIndex;
	task.Param_1 = (DWORD_PTR)_NewObject;

	CTaskMgr::GetInst()->AddTask(task);
}

void DeleteObject(CGameObject* _DeleteObject)
{
	tTask task = {};
	task.Type = TASK_TYPE::DELETE_OBJECT;
	task.Param_0 = (DWORD_PTR)_DeleteObject;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevelState(LEVEL_STATE _NextState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVELSTATE;
	task.Param_0 = (DWORD_PTR)_NextState;

	CTaskMgr::GetInst()->AddTask(task);
}

void ChangeLevel(CLevel* _Level, LEVEL_STATE _NextLevelState)
{
	tTask task = {};
	task.Type = TASK_TYPE::CHANGE_LEVEL;

	task.Param_0 = (DWORD_PTR)_Level;
	task.Param_1 = (DWORD_PTR)_NextLevelState;

	CTaskMgr::GetInst()->AddTask(task);
}

bool IsValid(CGameObject*& _Object)
{
	if (_Object == nullptr)
		return false;

	if (_Object->IsDead())
	{
		_Object = nullptr;
		return false;
	}

	return true;
}

bool IsPlatformLayerObject(CGameObject* _Object)
{
	LAYER_TYPE layer = LAYER_TYPE(_Object->GetLayerIdx());

	if (layer == LAYER_TYPE::PLATFORM0 || layer == LAYER_TYPE::PLATFORM1
	 || layer == LAYER_TYPE::PLATFORM2 || layer == LAYER_TYPE::PLATFORM3)
		return true;
	else
		return false;
}

bool IsMapLimitObject(CGameObject* _Object)
{
	UINT layer = _Object->GetLayerIdx();

	if (layer == (UINT)LAYER_TYPE::MAPLIMIT_COLLIDER)
		return true;
	else
		return false;
}

//bool IsAttackableObject(CGameObject* _Object)          // Player -> Object 공격 가능한 대상인지에 대한 여부
//{
//}
//
//bool IsDamageableObject(CGameObject* _Object)         // Object -> Player 데미지를 받을 수 있는 대상인지에 대한 여부
//{
//	UINT layer = _Object->GetLayerIdx();
//
//
//}

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.vPos = _Pos;
	Info.vScale = _Scale;
	Info.vRot = _Rot;
	Info.LifeTime = _Life;

	Info.matWorld = XMMatrixScaling(_Scale.x, _Scale.y, _Scale.z)
		* XMMatrixRotationX(_Rot.x)
		* XMMatrixRotationY(_Rot.y)
		* XMMatrixRotationZ(_Rot.z)
		* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);

	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::RECT;
	Info.matWorld = _matWorld;
	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;
	Info.LifeTime = _Life;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void DrawDebugCircle(Vec3 _Pos, float _Radius, Vec4 _Color, float _Life, bool _DepthTest)
{
	tDebugShapeInfo Info = {};

	Info.Shape = DEBUG_SHAPE::CIRCLE;
	Info.vPos = _Pos;
	Info.vScale = Vec3(_Radius * 2.f, _Radius * 2.f, 1.f);
	Info.LifeTime = _Life;

	Info.matWorld = XMMatrixScaling(Info.vScale.x, Info.vScale.y, Info.vScale.z)
					* XMMatrixTranslation(_Pos.x, _Pos.y, _Pos.z);

	Info.vColor = _Color;
	Info.DepthTest = _DepthTest;

	CRenderMgr::GetInst()->AddDebugShapeInfo(Info);
}

void SaveWString(const wstring& _String, FILE* _File)
{
	size_t len = _String.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_String.c_str(), sizeof(wchar_t), len, _File);
}

void SaveStringToFile(const string& _String, FILE* _File)
{
	size_t len = _String.length();
	fwrite(&len, sizeof(size_t), 1, _File);
	fwrite(_String.c_str(), sizeof(char), len, _File);
}

void LoadWString(wstring& _String, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);

	_String.resize(len);
	fread((wchar_t*)_String.c_str(), sizeof(wchar_t), len, _File);
}

void LoadStringFromFile(string& _String, FILE* _File)
{
	size_t len = 0;
	fread(&len, sizeof(size_t), 1, _File);

	_String.resize(len);
	fread((char*)_String.c_str(), sizeof(char), len, _File);
}