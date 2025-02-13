#pragma once

void CreateObject(class CGameObject* _NewObject, int _LayerIndex);
void DeleteObject(CGameObject* _DeleteObject);
void ChangeLevelState(LEVEL_STATE _NextState);
void ChangeLevel(class CLevel* _Level, LEVEL_STATE _NextLevelState);

bool IsValid(CGameObject*& _Object);

bool IsPlatformLayerObject(CGameObject* _Object);
bool IsMapLimitObject(CGameObject* _Object);

void DrawDebugRect(Vec3 _Pos, Vec3 _Scale, Vec3 _Rot, Vec4 _Color, float _Life, bool _DepthTest);
void DrawDebugRect(const Matrix& _matWorld, Vec4 _Color, float _Life, bool _DepthTest);

// 2D Debug
// void DrawDebugCircle();
// void DrawDebugLine();

// 3D Debug
// void DrawDebugCube();
// void DrawDebugSphere();

void SaveWString(const wstring& _String, FILE* _File);
void LoadWString(wstring& _String, FILE* _File);


template<typename T>
void Delete_Vec(vector<T*>& _vec)
{
	for (size_t i = 0; i < _vec.size(); ++i)
	{
		if (_vec[i] != nullptr)
			delete _vec[i];
	}
	_vec.clear();
}

template<typename T1, typename T2>
void Delete_Map(map<T1, T2>& _map)
{
	for (const auto& pair : _map)
	{ 
		if (pair.second != nullptr)
			delete pair.second;
	}
}

template<typename T, int _Size>
void Delete_Array(T(&_arr)[_Size])
{
	for (int i = 0; i < _Size; ++i)
	{
		if (_arr[i] != nullptr)
			delete _arr[i];
	}
}


#include "assets.h"
template<typename T>
ASSET_TYPE GetAssetType()
{
	if constexpr (std::is_same_v<T, CMesh>)
		return ASSET_TYPE::MESH;

	/*if constexpr (std::is_same_v<T, CMeshData>)
		return ASSET_TYPE::MESH_DATA;*/

	if constexpr (std::is_same_v<T, CMaterial>)
		return ASSET_TYPE::MATERIAL;

	if constexpr (std::is_same_v<T, CPrefab>)
		return ASSET_TYPE::PREFAB;

	if constexpr (std::is_same_v<T, CTexture>)
		return ASSET_TYPE::TEXTURE;

	if constexpr (std::is_same_v<T, CSound>)
		return ASSET_TYPE::SOUND;

	if constexpr (std::is_same_v<T, CGraphicShader>)
		return ASSET_TYPE::GRAPHIC_SHADER;

	if constexpr (std::is_same_v<T, CComputeShader>)
		return ASSET_TYPE::COMPUTE_SHADER;

	if constexpr (std::is_same_v<T, CSprite>)
		return ASSET_TYPE::SPRITE;

	if constexpr (std::is_same_v<T, CFlipBook>)
		return ASSET_TYPE::FLIPBOOK;
}