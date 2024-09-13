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
	"DEFAULT", 	                      // 0
	"MAPLIMIT_COLLIDER",              // 1
	"BACKGROUND3",	                  // 2
	"BACKGROUND2",              	  // 3
	"BACKGROUND1",	                  // 4
	"BACKGROUND0",	                  // 5
	"BACKGROUND_DECO",           	  // 6
	"PLATFORM3",                	  // 7
	"PLATFORM2",                	  // 8
	"LANDMARK1",                	  // 9
	"LANDMARK0",	                  // 10
	"PLATFORM1",	                  // 11
	"PLATFORM0",	                  // 12
	"ITEMBOX",	                      // 13
	"LIGHT3",	                      // 14
	"LIGHT2",	                      // 15
	"LIGHT1",	                      // 16
	"LIGHT0",	                      // 17
	"MONSTER",	                      // 18
	"DAMAGE_EFFECT",	              // 19
	"WEAPON_EFFECT",	              // 20
	"PLAYER",                    	  // 21
	"PLAYERWEAPON",                   // 22
	"PLAYER_PROJECTTILE",    	      // 23
	"MONSTER_PROJECTTILE", 	  	      // 24
	"FOREGROUND",                	  // 25
	"POSTPROCESS",               	  // 26
	"UI",                        	  // 27
	"",						          // 28
	"",						          // 29
	"",						          // 30
	""   					          // 31
};
const char* ToString(LAYER_TYPE _Type) { return LAYER_TYPE_STRING[(UINT)_Type]; }
const char* ToString(int _Index) {return LAYER_TYPE_STRING[_Index]; }