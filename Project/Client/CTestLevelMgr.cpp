#include "pch.h"
#include "CTestLevelMgr.h"

#include <Engine/CAssetMgr.h>
#include <Engine/assets.h>

#include <Engine/CLevelMgr.h>
#include <Engine/CLevel.h>
#include <Engine/CLayer.h>
#include <Engine/CGameObject.h>
#include <Engine/components.h>

#include <Engine/CCollisionMgr.h>

#include <Scripts/CPlayerScript.h>
#include <Scripts/CMissileScript.h>
#include <Scripts/CCameraMoveScript.h>

#include "CLevelSaveLoad.h"


void CTestLevelMgr::CreateTestLevel()
{
	// Material 로딩
	Ptr<CMaterial> pStdMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl");
	Ptr<CMaterial> pAlphaBlendMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DAlphaBlendMtrl");
	Ptr<CMaterial> pOneOneMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DOneOneMtrl");
	Ptr<CMaterial> pDebugShapeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl");

	// Texture 로딩
	Ptr<CTexture> pPlayerTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Scary_Idle_0.png");
	Ptr<CTexture> pMonsterTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Zombi_Attack_Atlas_0.png");

	// Prefab 생성
	CreatePrefab();

	// Level 생성
	CLevel* pLevel = new CLevel;

	// Level 지정
	ChangeLevel(pLevel, LEVEL_STATE::PLAY);

	// Camera Object
	CGameObject* CamObj = new CGameObject;
	CamObj->SetName(L"MainCamera");
	CamObj->AddComponent(new CTransform);
	CamObj->AddComponent(new CCamera);
	CamObj->AddComponent(new CCameraMoveScript);

	CamObj->Transform()->SetRelativePos(0, 0, -150.f);

	CamObj->Camera()->SetPriority(0);
	CamObj->Camera()->SetLayerAll();
	CamObj->Camera()->SetLayer(31, false);
	CamObj->Camera()->SetFar(10000.f);
	CamObj->Camera()->SetProjType(PROJ_TYPE::PERSPECTIVE);

	pLevel->AddObject(0, CamObj);


	CGameObject* pObject = nullptr;


	// Light2D Object
	pObject = new CGameObject;
	pObject->SetName(L"PointLight");
	pObject->AddComponent(new CTransform);
	pObject->AddComponent(new CLight2D);

	pObject->Light2D()->SetLightType(LIGHT_TYPE::POINT);
	pObject->Light2D()->SetLightColor(Vec3(1.f, 1.f, 1.f));
	pObject->Light2D()->SetRadius(1000.f);
	pObject->Transform()->SetRelativePos(Vec3(0.f, 0.f, 100.f));

	pLevel->AddObject(0, pObject);


	// Player Object
	CGameObject* pPlayer = new CGameObject;
	pPlayer->SetName(L"Player");
	pPlayer->AddComponent(new CTransform);
	pPlayer->AddComponent(new CMeshRender);
	pPlayer->AddComponent(new CCollider2D);
	pPlayer->AddComponent(new CFlipBookComponent);
	pPlayer->AddComponent(new CPlayerScript);

	pPlayer->Transform()->SetRelativePos(0.f, 0.f, 100.f);
	pPlayer->Transform()->SetRelativeScale(200.f, 200.f, 1.f);

	pPlayer->Collider2D()->SetIndependentScale(false);
	pPlayer->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	pPlayer->Collider2D()->SetScale(Vec3(1.f, 1.f, 1.f));

	pPlayer->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));

	// Std Mtrl
	pStdMtrl->SetTexParam(TEX_0, pPlayerTexture);
	pPlayer->MeshRender()->SetMaterial(pStdMtrl);

	// AlphaBlend Mtrl
	//pAlphaBlendMtrl->SetTexParam(TEX_0, pPlayerTexture);
	//pObject->MeshRender()->SetMaterial(pAlphaBlendMtrl);

	// One_One Mtrl
	//pOneOneMtrl->SetTexParam(TEX_0, pPlayerTexture);
	//pObject->MeshRender()->SetMaterial(pOneOneMtrl);

	// Debug Mtrl
	//pObject->MeshRender()->SetMaterial(pDebugShapeMtrl);
	//pObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, Vec4(0.f, 1.f, 0.f, 1.f));
	//pObject->MeshRender()->GetMaterial()->SetScalarParam(FLOAT_0, 0.01f);
	//pObject->MeshRender()->GetMaterial()->SetScalarParam(INT_1, 1);

	Ptr<CFlipBook> pFlipBook = CAssetMgr::GetInst()->FindAsset<CFlipBook>(L"animation\\Caballos_Idle.flip");
	pPlayer->FlipBookComponent()->AddFlipBook(5, pFlipBook);
	pPlayer->FlipBookComponent()->Play(5, true);

	pLevel->AddObject(3, pPlayer);

	// 복사 -> 추가가 가능하도록 구현해보자!
	//CGameObject* pClonePlayer = pPlayer->Clone();
	//pLevel->AddObject(3, pClonePlayer);

	// Monster Object
	//CGameObject* pMonster = new CGameObject;
	//pMonster->SetName(L"Monster");
	//pMonster->AddComponent(new CTransform);
	//pMonster->AddComponent(new CMeshRender);
	//pMonster->AddComponent(new CCollider2D);
	//
	//pMonster->Transform()->SetRelativePos(-400.f, 0.f, 100.f);
	//pMonster->Transform()->SetRelativeScale(150.f, 150.f, 1.f);
	//
	//pMonster->Collider2D()->SetOffset(Vec3(0.f, 0.f, 0.f));
	//pMonster->Collider2D()->SetScale(Vec3(1.2f, 1.2f, 1.f));
	//
	//pMonster->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pAlphaBlendMtrl->SetTexParam(TEX_0, pMonsterTexture);
	//pMonster->MeshRender()->SetMaterial(pAlphaBlendMtrl);
	//
	//pLevel->AddObject(4, pMonster);


	// TileMap Object
	//CGameObject* pTileMapObj = new CGameObject;
	//pTileMapObj->SetName(L"TileMap");
	//
	//pTileMapObj->AddComponent(new CTransform);
	//pTileMapObj->AddComponent(new CTileMap);
	//
	//pTileMapObj->Transform()->SetRelativePos(Vec3(0.f, 200.f, 200.f));
	//pTileMapObj->TileMap()->SetRowCol(20, 20);
	//pTileMapObj->TileMap()->SetTileSize(Vec2(20.f, 20.f));
	//pTileMapObj->TileMap()->SetTileInfo(3);
	//
	//Ptr<CTexture> pTileAtlas = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\TilemapEx2.bmp");
	//pTileMapObj->TileMap()->SetAtlasTexture(pTileAtlas);
	//pTileMapObj->TileMap()->SetAtlasTileSize(Vec2(20.f, 20.f));
	//
	//pLevel->AddObject(2, pTileMapObj);


	// PostProcess Object
	//CGameObject* pPostProcessObj = new CGameObject;
	//pPostProcessObj->SetName(L"PostProcess");
	//pPostProcessObj->AddComponent(new CTransform);
	//pPostProcessObj->AddComponent(new CMeshRender);
	//
	//pPostProcessObj->Transform()->SetRelativePos(150.f, 150.f, 1.f);
	//pPostProcessObj->Transform()->SetRelativeScale(500.f, 500.f, 1.f);
	//
	//pPostProcessObj->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	//pPostProcessObj->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"RippleMtrl"));
	//
	//pLevel->AddObject(0, pPostProcessObj);

	// Collision 지정
	CCollisionMgr::GetInst()->CollisionCheck(3, 4);   // Player vs Monster
	CCollisionMgr::GetInst()->CollisionCheck(5, 4);   // Player Projectile vs Monster
}

void CTestLevelMgr::CreatePrefab()
{
	CGameObject* pProto = new CGameObject;
	pProto->SetName(L"Missile");
	pProto->AddComponent(new CTransform);
	pProto->AddComponent(new CMeshRender);
	pProto->AddComponent(new CMissileScript);

	pProto->Transform()->SetRelativeScale(100.f, 100.f, 1.f);

	pProto->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh"));
	pProto->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"Std2DMtrl"));

	Ptr<CPrefab> pPrefab = new CPrefab;
	pPrefab->SetProtoObject(pProto);

	CAssetMgr::GetInst()->AddAsset<CPrefab>(L"MissilePref", pPrefab);

	wstring FilePath = CPathMgr::GetInst()->GetContentPath();
	FilePath += L"prefab\\Missile.pref";
	pPrefab->Save(FilePath);
}