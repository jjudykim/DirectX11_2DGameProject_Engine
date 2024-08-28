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
	"POSTPROCESS",                // 0
	"FOREGROUND",                 // 1
	"MONSTER_PROJECTTILE",        // 2
	"PLAYER_PROJECTTILE",   	  // 3
	"PLAYERWEAPON",         	  // 4
	"PLAYER",               	  // 5
	"WEAPON_EFFECT",		      // 6
	"DAMAGE_EFFECT",		      // 7
	"MONSTER",		              // 8
	"LIGHT0",		              // 9
	"LIGHT1",		              // 10
	"LIGHT2",		              // 11
	"LIGHT3",		              // 12
	"ITEMBOX",		              // 13
	"PLATFORM0",		          // 14
	"PLATFORM1",		          // 15
	"LANDMARK0",		          // 16
	"LANDMARK1",                  // 17
	"PLATFORM2",                  // 18
	"PLATFORM3",                  // 19
	"BACKGROUND_DECO",		      // 20
	"BACKGROUND0",		          // 21
	"BACKGROUND1",		          // 22
	"BACKGROUND2",                // 23
	"BACKGROUND3",		          // 24
	"DEFAULT",                    // 25
	"",						      // 26
	"",						      // 27
	"",						      // 28
	"",						      // 29
	"",						      // 30
	""   					      // 31
};
const char* ToString(LAYER_TYPE _Type) { return LAYER_TYPE_STRING[(UINT)_Type]; }