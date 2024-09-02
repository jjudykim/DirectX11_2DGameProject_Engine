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
	"UI",                         // 0
	"POSTPROCESS",                // 1
	"FOREGROUND",                 // 2
	"MONSTER_PROJECTTILE",  	  // 3
	"PLAYER_PROJECTTILE",   	  // 4
	"PLAYERWEAPON",         	  // 5
	"PLAYER",                     // 6
	"WEAPON_EFFECT",		      // 7
	"DAMAGE_EFFECT",		      // 8
	"MONSTER",		              // 9
	"LIGHT0",		              // 10
	"LIGHT1",		              // 11
	"LIGHT2",		              // 12
	"LIGHT3",		              // 13
	"ITEMBOX",		              // 14
	"PLATFORM0",		          // 15
	"PLATFORM1",		          // 16
	"LANDMARK0",		          // 17
	"LANDMARK1",                  // 18
	"PLATFORM2",                  // 19
	"PLATFORM3",                  // 20
	"BACKGROUND_DECO",		      // 21
	"BACKGROUND0",		          // 22
	"BACKGROUND1",		          // 23
	"BACKGROUND2",                // 24
	"BACKGROUND3",		          // 25
	"MAPLIMIT_COLLIDER",          // 26
	"DEFAULT", 					  // 27
	"",						      // 28
	"",						      // 29
	"",						      // 30
	""   					      // 31
};
const char* ToString(LAYER_TYPE _Type) { return LAYER_TYPE_STRING[(UINT)_Type]; }
const char* ToString(int _Index) {return LAYER_TYPE_STRING[_Index]; }