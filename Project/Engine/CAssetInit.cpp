#include "pch.h"
#include "CAssetMgr.h"

#include "CAsset.h"
#include "CDevice.h"

#include "CSetColorCS.h"
#include "CStructuredBuffer.h"

void CAssetMgr::Init()
{
	CreateEngineMesh();
	CreateEngineSprite();
	CreateEngineTexture();
	CreateEngineGraphicShader();
	CreateEngineComputeShader();
	CreateEngineMaterial();
}

void CAssetMgr::Tick()
{
	if (m_Changed)
		m_Changed = false;
}

void CAssetMgr::CreateEngineMesh()
{
	Ptr<CMesh> pMesh = nullptr;
	Vtx v;

	// PointMesh
	pMesh = new CMesh;
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vColor = Vec4(0.f, 0.f, 0.f, 1.f);
	v.vUV = Vec2(0.f, 0.f);

	UINT i = 0;
	pMesh->Create(&v, 1, &i, 1);
	AddAsset(L"PointMesh", pMesh);

	// Rect Mesh 생성
	Vtx arrVtx[4] = {};

	arrVtx[0].vPos = Vec3(-0.5f, 0.5f, 0.f);
	arrVtx[0].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[0].vUV = Vec2(0.f, 0.f);

	arrVtx[1].vPos = Vec3(0.5f, 0.5f, 0.f);
	arrVtx[1].vColor = Vec4(0.f, 1.f, 0.f, 1.f);
	arrVtx[1].vUV = Vec2(1.f, 0.f);

	arrVtx[2].vPos = Vec3(0.5f, -0.5f, 0.f);
	arrVtx[2].vColor = Vec4(0.f, 0.f, 1.f, 1.f);
	arrVtx[2].vUV = Vec2(1.f, 1.f);

	arrVtx[3].vPos = Vec3(-0.5f, -0.5f, 0.f);
	arrVtx[3].vColor = Vec4(1.f, 0.f, 0.f, 1.f);
	arrVtx[3].vUV = Vec2(0.f, 1.f);

	// Rect Mesh - Index 버퍼 생성
	UINT arrIdx[6] = {};
	arrIdx[0] = 0;	arrIdx[1] = 1;	arrIdx[2] = 2;
	arrIdx[3] = 0; 	arrIdx[4] = 2;	arrIdx[5] = 3;

	// Rect Mesh - Mesh 생성
	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 6);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh", pMesh);
	

	// RectMesh_Debug 생성
	arrIdx[0] = 0;   arrIdx[1] = 1;   arrIdx[2] = 2;
	arrIdx[3] = 3;   arrIdx[4] = 0;

	pMesh = new CMesh;
	pMesh->Create(arrVtx, 4, arrIdx, 5);
	pMesh->SetEngineAsset();
	AddAsset(L"RectMesh_Debug", pMesh);


	// CircleMesh 생성
	vector<Vtx> vecVtx;
	vector<UINT> vecIdx;

	int Slice = 40;
	float fTheta = XM_2PI / Slice;
	float Radius = 0.5f;

	// Circle Mesh - 중심점
	v.vPos = Vec3(0.f, 0.f, 0.f);
	v.vUV = Vec2(0.5f, 0.5f);
	v.vColor = Vec4(1.f, 1.f, 1.f, 1.f);
	vecVtx.push_back(v);

	// Circle Mesh - 테두리
	float Angle = 0.f;
	for (int i = 0; i < Slice + 1; ++i)
	{
		v.vPos = Vec3(Radius * cosf(Angle), Radius * sinf(Angle), 0.f);
		v.vUV = Vec2(v.vPos.x + 0.5f, -(v.vPos.y - 0.5f));
		vecVtx.push_back(v);

		Angle += fTheta;
	}

	// Circle Mesh - 인덱스
	for (int i = 0; i < Slice; ++i)
	{
		vecIdx.push_back(0);
		vecIdx.push_back(i + 2);
		vecIdx.push_back(i + 1);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh", pMesh);


	// CircleMesh_Debug 생성
	vecIdx.clear();
	for (size_t i = 0; i < vecVtx.size(); ++i)
	{
		vecIdx.push_back((UINT)i);
	}

	pMesh = new CMesh;
	pMesh->Create(vecVtx.data(), (UINT)vecVtx.size(), vecIdx.data(), (UINT)vecIdx.size());
	pMesh->SetEngineAsset();
	AddAsset(L"CircleMesh_Debug", pMesh);
}

float g_Scale = 0.25f;

void CAssetMgr::CreateEngineTexture()
{
	// PostProcess 용도 Texture 생성
	Vec2 Resolution = CDevice::GetInst()->GetResolution();
	Ptr<CTexture> pPostProcessTex = CreateTexture(L"PostProcessTex"
												, (UINT)Resolution.x, (UINT)Resolution.y
												, DXGI_FORMAT_R8G8B8A8_UNORM, D3D10_BIND_SHADER_RESOURCE);
	
	// Blur 용도 Texture 생성

	// 0.25
	Ptr<CTexture> pDownScaleTex1 = CreateTexture(
		L"DownScaleTex_1" 
		, (UINT)(Resolution.x * 0.25f), (UINT)(Resolution.y * 0.25f)
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	
	// 0.5
	Ptr<CTexture> pDownScaleTex2 = CreateTexture(
		L"DownScaleTex_2" 
		, (UINT)(Resolution.x * 0.5f), (UINT)(Resolution.y * 0.5f)
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);
	
	// 0.75
	Ptr<CTexture> pDownScaleTex3 = CreateTexture(
		L"DownScaleTex_3" 
		, (UINT)(Resolution.x * 0.75f), (UINT)(Resolution.y * 0.75f)
		, DXGI_FORMAT_R8G8B8A8_UNORM, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


	Ptr<CTexture> pEffectTarget = CreateTexture(
		L"EffectTargetTex"
		, (UINT)(Resolution.x), (UINT)(Resolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


	Ptr<CTexture> pEffectDepth = CreateTexture(
		L"EffectDepthStencilTex"
		, (UINT)(Resolution.x), (UINT)(Resolution.y)
		, DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_BIND_DEPTH_STENCIL);


	Ptr<CTexture> pEffectBlurTarget = CreateTexture(
		L"EffectBlurTargetTex"
		, (UINT)(Resolution.x), (UINT)(Resolution.y)
		, DXGI_FORMAT_R32G32B32A32_FLOAT, D3D11_BIND_RENDER_TARGET | D3D11_BIND_SHADER_RESOURCE);


	// Noise Texture
	Load<CTexture>(L"texture\\noise\\noise_01.png", L"texture\\noise\\noise_01.png");
	Load<CTexture>(L"texture\\noise\\noise_02.png", L"texture\\noise\\noise_02.png");
	Load<CTexture>(L"texture\\noise\\noise_03.jpg", L"texture\\noise\\noise_03.jpg");
}

void CAssetMgr::CreateEngineSprite()
{
	// wstring strContentPath = CPathMgr::GetInst()->GetContentPath();

	// Save 하는 과정
	//Ptr<CTexture> pAtlasTex = Load<CTexture>(L"texture\\Scary_Idle.png", L"texture\\Scary_Idle.png");
	//Ptr<CSprite> pSprite = nullptr;
	//
	//for (int i = 0; i < 11; ++i)
	//{
	//	wchar_t szKey[50] = {};
	//	swprintf_s(szKey, 50, L"Scary_Idle_%d", i);
	//
	//	pSprite = new CSprite;
	//	pSprite->Create(pAtlasTex, Vec2((i % 4) * 520.f, (i / 4) * 450.f), Vec2(520.f, 450.f));
	//	pSprite->SetBackground(Vec2(450.f, 450.f));
	//
	//	pSprite->SetRelativePath(wstring(L"animation\\") + szKey + L".sprite");
	//	pSprite->Save(strContentPath + L"Animation\\" + szKey + L".sprite");
	//	
	//	AddAsset(szKey, pSprite);
	//}
	//
	//Ptr<CFlipBook> pFlipBook = nullptr;
	//pFlipBook = new CFlipBook;
	//
	//for (int i = 0; i < 11; ++i)
	//{
	//	wchar_t szKey[50] = {};
	//	swprintf_s(szKey, 50, L"Scary_Idle_%d", i);
	//	pFlipBook->AddSprite(FindAsset<CSprite>(szKey));
	//}
	//
	//AddAsset(L"Scary_Idle", pFlipBook);
	//wstring path = wstring(L"animation\\") + L"Scary_Idle" + L".flip";
	//pFlipBook->SetRelativePath(path);
	//pFlipBook->Save(strContentPath + path);

	// Load 하는 과정
	//Ptr<CFlipBook> pFlipBook = new CFlipBook;
	//Load<CFlipBook>(L"Scary_Idle", wstring(L"animation\\") + L"Scary_Idle" + L".flip");
}

void CAssetMgr::CreateEngineGraphicShader()
{
	// Shader 생성
	Ptr<CGraphicShader> pShader = nullptr;

	// Std2DShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	// Current Tex Param
	pShader->AddScalarParam(INT_0, "Current Tex Param Index");

	for (UINT i = 0; i < 16; ++i)
	{
		pShader->AddTexParam((TEX_PARAM)i, "Texture" + std::to_string(i + 1));
	}

	AddAsset(L"Std2DShader", pShader);


	// Std2DCustomBlend
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Alphablend");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	//pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::CUSTOM_BLEND);                                 // 알파 블렌딩 강도 조절
	
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	// Current Tex Param
	pShader->AddScalarParam(INT_0, "Current Tex Param Index");

	for (UINT i = 0; i < 16; ++i)
	{
		pShader->AddTexParam((TEX_PARAM)i, "Texture" + std::to_string(i + 1));
	}

	AddAsset(L"Std2DCustomBlendShader", pShader);


	// Std2DAlphaBlend
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_Alphablend");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"Std2DAlphaBlendShader", pShader);


	// Std2DOneOne
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetBSType(BS_TYPE::ONE_ONE);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_OPAQUE);

	AddAsset(L"Std2DOneOneShader", pShader);


	// Std2DForUIShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2DForUI");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	// Current Tex Param
	pShader->AddScalarParam(INT_0, "Cur Tex Param Index");

	for (UINT i = 0; i < 16; ++i)
	{
		pShader->AddTexParam((TEX_PARAM)i, "Texture" + std::to_string(i + 1));
	}

	AddAsset(L"Std2DForUIShader", pShader);


	// Std2DCustomBlendForUI
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Std2D");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Std2D_AlphablendForUI");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS_EQUAL);
	pShader->SetBSType(BS_TYPE::CUSTOM_BLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_UI);

	// Current Tex Param
	pShader->AddScalarParam(INT_0, "Cur Tex Param Index");

	for (UINT i = 0; i < 16; ++i)
	{
		pShader->AddTexParam((TEX_PARAM)i, "Texture" + std::to_string(i + 1));
	}

	AddAsset(L"Std2DCustomBlendForUIShader", pShader);


	// EffectShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_EFFECT);

	AddAsset(L"EffectShader", pShader);

	pShader->AddScalarParam(VEC4_0, "EffectColor");
	pShader->AddTexParam(TEX_0, "EffectTexture");


	// EffectShader_TransparentDomain
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\std2d.fx", "VS_Effect");
	pShader->CreatePixelShader(L"shader\\std2d.fx", "PS_Effect");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_TRANSPARENT);

	AddAsset(L"EffectShader_TD", pShader);

	pShader->AddScalarParam(VEC4_0, "EffectColor");
	pShader->AddTexParam(TEX_0, "EffectTexture");



	// DebugShapeShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\debug.fx", "VS_DebugShape");
	pShader->CreatePixelShader(L"shader\\debug.fx", "PS_DebugShape");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_LINESTRIP);

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_DEBUG);

	AddAsset(L"DebugShapeShader", pShader);


	// TileMapShader
	pShader = new CGraphicShader;

	pShader->CreateVertexShader(L"shader\\tilemap.fx", "VS_TileMap");
	pShader->CreatePixelShader(L"shader\\tilemap.fx", "PS_TileMap");

	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::LESS);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_MASKED);

	AddAsset(L"TileMapShader", pShader);


	//ParticleShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\particle.fx", "VS_Particle");
	pShader->CreateGeometryShader(L"shader\\particle.fx", "GS_Particle");
	pShader->CreatePixelShader(L"shader\\particle.fx", "PS_Particle");

	pShader->SetTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_PARTICLE);

	AddAsset(L"ParticleRenderShader", pShader);


	// BlurShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Blur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Blur");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"BlurShader", pShader);


	// EffectMerge
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_EffectMerge");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_EffectMerge");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);
	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	AddAsset(L"EffectMergeShader", pShader);


	// PostProcess - GrayFilterShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_GrayFilter");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_GrayFilter");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"GrayFilterShader", pShader);


	// PostProcess - DistortionShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Distortion");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Distortion");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);
	
	AddAsset(L"DistortionShader", pShader);


	// PostProcess - Practice) RippleShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Ripple");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Ripple");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"RippleShader", pShader);

	// PostProcess - Practice) MeltingShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_Melting");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_Melting");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"MeltingShader", pShader);

	// PostProcess - Practice) MeltingShader
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_AnalogTV");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_AnalogTV");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::DEFAULT);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"AnalogTVShader", pShader);

	// PostProcess - Bokeh Blur
	pShader = new CGraphicShader;
	pShader->CreateVertexShader(L"shader\\postprocess.fx", "VS_BokehBlur");
	pShader->CreatePixelShader(L"shader\\postprocess.fx", "PS_BokehBlur");
	pShader->SetRSType(RS_TYPE::CULL_NONE);
	pShader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	pShader->SetBSType(BS_TYPE::ALPHABLEND);

	pShader->SetDomain(SHADER_DOMAIN::DOMAIN_POSTPROCESS);

	AddAsset(L"BokehBlurShader", pShader);

}

#include "CParticleSystem.h"

void CAssetMgr::CreateEngineComputeShader()
{
	// ParticleTick
	Ptr<CComputeShader> pCS = nullptr;

	pCS = new CParticleTickCS;
	AddAsset<CComputeShader>(L"ParticleTickCS", pCS);
}

void CAssetMgr::CreateEngineMaterial()
{
	// Material 생성
	Ptr<CMaterial> pMtrl = nullptr;

	// Std2DMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DShader"));
	AddAsset(L"Std2DMtrl", pMtrl);

	// Std2DAlphaBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DAlphaBlendShader"));
	AddAsset(L"Std2DAlphaBlendMtrl", pMtrl);

	// Std2DOneOneMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DOneOneShader"));
	AddAsset(L"Std2DOneOneMtrl", pMtrl);

	// Std2DCustomBlendMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"Std2DCustomBlendShader"));
	AddAsset(L"Std2DCustomBlendMtrl", pMtrl);

	// EffectMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectShader"));
	AddAsset(L"EffectMtrl", pMtrl);

	pMtrl->SetScalarParam(VEC4_0, Vec4(0.f, 0.f, 0.f, 0.f));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Default_ParticleSystem.png"));


	// Effect_TD
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectShader_TD"));
	AddAsset(L"EffectTDMtrl", pMtrl);

	pMtrl->SetScalarParam(VEC4_0, Vec4(0.f, 0.f, 0.f, 0.f));
	pMtrl->SetTexParam(TEX_0, CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\Default_ParticleSystem.png"));

	// DebugShapeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DebugShapeShader"));
	AddAsset(L"DebugShapeMtrl", pMtrl);

	// TileMapMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"TileMapShader"));
	AddAsset(L"TileMapMtrl", pMtrl);

	// BlurMtrl1
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	pMtrl->SetScalarParam(FLOAT_0, 0.25f);
	AddAsset(L"BlurMtrl1", pMtrl);

	// BlurMtrl2
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	pMtrl->SetScalarParam(FLOAT_0, 0.5f);
	AddAsset(L"BlurMtrl2", pMtrl);

	// BlurMtrl3
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BlurShader"));
	pMtrl->SetScalarParam(FLOAT_0, 0.75f);
	AddAsset(L"BlurMtrl3", pMtrl);

	// EffectMergeMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"EffectMergeShader"));
	AddAsset(L"EffectMergeMtrl", pMtrl);

	// PostProcess - GrayFilterMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"GrayFilterShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"GrayFilterMtrl", pMtrl);

	// PostProcess - DistortionMtrl
	pMtrl = new CMaterial();
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"DistortionShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	pMtrl->SetTexParam(TEX_1, FindAsset<CTexture>(L"texture\\noise\\noise_01.png"));
	pMtrl->SetTexParam(TEX_2, FindAsset<CTexture>(L"texture\\noise\\noise_02.png"));
	pMtrl->SetTexParam(TEX_3, FindAsset<CTexture>(L"texture\\noise\\noise_03.jpg"));
	AddAsset(L"DistortionMtrl", pMtrl);

	// PostProcess - Practice) RippleMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"RippleShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"RippleMtrl", pMtrl);

	// PostProcess - Practice) MeltingMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"MeltingShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"MeltingMtrl", pMtrl);

	// PostProcess - Practice) AnalogTVMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"AnalogTVShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"AnalogTVMtrl", pMtrl);

	// PostProcess - Bokeh Blur
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"BokehBlurShader"));
	pMtrl->SetTexParam(TEX_0, FindAsset<CTexture>(L"PostProcessTex"));
	AddAsset(L"BokehBlurMtrl", pMtrl);

	// ParticleRenderMtrl
	pMtrl = new CMaterial(true);
	pMtrl->SetShader(FindAsset<CGraphicShader>(L"ParticleRenderShader"));
	AddAsset(L"ParticleRenderMtrl", pMtrl);
}

