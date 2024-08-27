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
	"BACKGROUND0",                  // 1
	"BACKGROUND1",                  // 2
	"BACKGROUND2",					// 3
	"BACKGROUND3",					// 4
	"BACKGROUND_DECO",				// 5
	"PLATFORM"						// 6
	"ITEMBOX"						// 7
	"LIGHT0",						// 8
	"LIGHT1",						// 9
	"LIGHT2",						// 10
	"LIGHT3",						// 11
	"MONSTER",						// 12
	"DAMAGE_EFFECT",				// 13
	"WEAPON_EFFECT",				// 14
	"PLAYER",                       // 15
	"PLAYERWEAPON",                 // 16
	"PLAYER_PROJECTTILE",           // 17
	"MONSTER_PROJECTTILE",			// 18
	"FOREGROUND",					// 19
	"POSTPROCESS",					// 20
	"UI",                           // 21
	"",								// 22
	"",								// 23
	"",                             // 24
	"",								// 25
	"",								// 26
	"",								// 27
	"",								// 28
	"",								// 29
	"",								// 30
	""   							// 31
};
const char* ToString(LAYER_TYPE _Type) { return LAYER_TYPE_STRING[(UINT)_Type]; }