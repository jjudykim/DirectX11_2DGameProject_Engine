#include "pch.h"
#include "CRenderMgr.h"

#include "CDevice.h"
#include "CConstBuffer.h"

#include "CCamera.h"
#include "CTimeMgr.h"
#include "CLogMgr.h"
#include "CAssetMgr.h"

#include "CGameObject.h"
#include "CTransform.h"
#include "CMeshRender.h"

#include "CLevelMgr.h"
#include "CLevel.h"

#include "CLight2D.h"
#include "CStructuredBuffer.h"

#include "CKeyMgr.h"

CRenderMgr::CRenderMgr()
	: m_DebugObject(nullptr)
	, m_EditorCamera(nullptr)
	, m_Light2DBuffer(nullptr)
{
	m_Light2DBuffer = new CStructuredBuffer;
}

CRenderMgr::~CRenderMgr()
{
	if (m_DebugObject != nullptr)
		delete m_DebugObject;

	if (m_Light2DBuffer != nullptr)
		delete m_Light2DBuffer;
}

void CRenderMgr::Init()
{
	// 만들어둔 PostProcess용 Texture를 참조
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");

	m_DownScaleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DownScaleTex");

	// Debug Render를 위한 Game Object
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// 렌더타겟 지정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	if (pCurLevel == nullptr)
	{
		float color[4] = { 0.f, 0.f, 0.f, 1.0f };
		CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), color);
		CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);
		return;
	}
		
	RenderStart();

	// Level이 Play 상태인 경우, Level 내에 있는 카메라 시점으로 렌더링
	if (pCurLevel->GetState() == PLAY)
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (m_vecCam[i] == nullptr)
				continue;

			m_vecCam[i]->Render();
		}
	}

	// Level이 Stop이나 Pause인 경우, Editor용 카메라 시점으로 렌더링
	else
	{
		if (m_EditorCamera != nullptr)
		{
			m_EditorCamera->Render();
		}
	}

	if (KEY_PRESSED(KEY::B))
	{
		Blur();
	}

	// Debug Render
	RenderDebugShape();

	// Time 정보 출력
	CTimeMgr::GetInst()->Render();
	CLogMgr::GetInst()->Render();

	// Clear
	Clear();
}

void CRenderMgr::RegisterCamera(CCamera* _Cam, int _CamPriority)
{
	if (m_vecCam.size() <= _CamPriority + 1)
		m_vecCam.resize(_CamPriority + 1);

	m_vecCam[_CamPriority] = _Cam;
}

void CRenderMgr::PostProcessCopy()
{
	// RenderTarget->PostProcessTex
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	CONTEXT->CopyResource(m_PostProcessTex->GetTex2D().Get(), pRTTex->GetTex2D().Get());
}

void CRenderMgr::Blur()
{
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");
	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl");

	CRenderMgr::PostProcessCopy();

	D3D11_VIEWPORT viewport = {};
	viewport.Width = m_DownScaleTex->Width();
	viewport.Height = m_DownScaleTex->Height();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, m_DownScaleTex->GetRTV().GetAddressOf(), nullptr);

	pBlurMtrl->SetTexParam(TEX_0, CRenderMgr::m_PostProcessTex);
	pBlurMtrl->Binding();
	pRectMesh->Render();

	// 렌더타겟 지정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");

	viewport.Width = pRTTex->Width();
	viewport.Height = pRTTex->Height();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	pBlurMtrl->SetTexParam(TEX_0, m_DownScaleTex);
	pBlurMtrl->Binding();
	pRectMesh->Render();
}

void CRenderMgr::RenderStart()
{
	// RenderTarget 지정
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// TargetClear
	float color[4] = { 0.f, 0.f, 0.f, 1.0f };
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), color);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = Vec2((float)pRTTex->Width(), (float)pRTTex->Height());
	g_GlobalData.g_Light2DCount = (int)m_vecLight2D.size();


	// Light2DInfo Update / Binding
	vector<tLightInfo> vecLight2DInfo;
	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	}

	if (vecLight2DInfo.size() == 0)
		vecLight2DInfo.push_back(tLightInfo{});

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	m_Light2DBuffer->SetData(vecLight2DInfo.data());
	m_Light2DBuffer->Binding(21);

	// GlobalData Binding
	static CConstBuffer* pGlobalCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::GLOBAL);
	pGlobalCB->SetData(&g_GlobalData);
	pGlobalCB->Binding();
}

void CRenderMgr::Clear()
{
	m_vecLight2D.clear();
}

void CRenderMgr::RenderDebugShape()
{
	list<tDebugShapeInfo>::iterator iter = m_DebugShapeList.begin();

	for (; iter != m_DebugShapeList.end();)
	{
		// Debug 요청 모양에 맞는 Mesh를 가져옴
		switch ((*iter).Shape)
		{
		case DEBUG_SHAPE::RECT:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh_Debug"));
			break;
		case DEBUG_SHAPE::CIRCLE:
			m_DebugObject->MeshRender()->SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"CircleMesh_Debug"));
			break;
		case DEBUG_SHAPE::LINE:

			break;
		case DEBUG_SHAPE::CUBE:

			break;
		case DEBUG_SHAPE::SPHERE:

			break;
		}

		// 위치 세팅
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// 재질 세팅
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS_EQUAL);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// 렌더링
		m_DebugObject->MeshRender()->Render();

		// 수명이 지난 디버그 정보 삭제
		(*iter).Age += EngineDT;
		if ((*iter).LifeTime < (*iter).Age)
		{
			iter = m_DebugShapeList.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}
