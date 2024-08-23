#include "pch.h"

tTransform  g_Trans = {};
tGlobalData g_GlobalData = {};

const char* ASSET_TYPE_STRING[] =
{
	"MESH",
	"MESH_DATA",
	"MATERIAL",
	"PREFAB",
	"TEXTURE",
	"SOUND",
	"GRAPHIC_SHADER",
	"COMPUTE_SHADER",
	"SPRITE",
	"FLIPBOOK",
};
const char* ToString(ASSET_TYPE _Type) { return ASSET_TYPE_STRING[(UINT)_Type]; }


const char* COMPONENT_TYPE_STRING[] =
{
	"TRANSFORM",
	"COLLIDER2D",
	"COLLIDER3D",
	"LIGHT2D",
	"LIGHT3D",
	"FLIPBOOKCOMPONENT",
	"ANIMATOR3D",
	"STATE_MACHINE",
	"RIGIDBODY",
	"CAMERA",
	"MESHRENDER",
	"TILEMAP",
	"PARTICLE_SYSTEM",
	"DECAl",
	"SKYBOX",
	"LANDSCAPE"
};
const char* ToString(COMPONENT_TYPE _Type) { return COMPONENT_TYPE_STRING[(UINT)_Type]; }


const char* LAYER_TYPE_STRING[MAX_LAYER] =
{
	"DEFAULT",                      // 0
	"BACKGROUND",                   // 1
	"MONSTER",                      // 2
	"PLAYER",						// 3
	"PLAYER_PROJECTTILE",           // 4
	"MONSTER_PROJECTTILE",			// 5
	"",								// 6
	"",								// 7
	"",                             // 8
	"",								// 9
	"",								// 10
	"",								// 11
	"",								// 12
	"",								// 13
	"",								// 14
	"",								// 15
	"",                             // 16
	"",								// 17
	"",								// 18
	"",								// 19
	"",                             // 20
	"",                             // 21
	"",                             // 22
	"",                             // 23
	"",                             // 24
	"",                             // 25
	"",                             // 26
	"",                             // 27
	"",                             // 28
	"",                             // 29
	"",                             // 30
	""                              // 31
};
const char* ToString(LAYER_TYPE _Type) { return LAYER_TYPE_STRING[(UINT)_Type]; }