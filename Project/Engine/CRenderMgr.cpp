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
	// ������ PostProcess�� Texture�� ����
	m_PostProcessTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"PostProcessTex");

	m_DownScaleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DownScaleTex_2");

	// Debug Render�� ���� Game Object
	m_DebugObject = new CGameObject;
	m_DebugObject->AddComponent(new CTransform);
	m_DebugObject->AddComponent(new CMeshRender);
	m_DebugObject->MeshRender()->SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"DebugShapeMtrl"));
}

void CRenderMgr::Tick()
{
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	// ����Ÿ�� ����
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

	// Level�� Play ������ ���, Level ���� �ִ� ī�޶� �������� ������
	if (pCurLevel->GetState() == PLAY)
	{
		for (size_t i = 0; i < m_vecCam.size(); ++i)
		{
			if (m_vecCam[i] == nullptr)
				continue;

			RenderLight(m_vecCam[i]);
			GlobalCBBinding();
			m_vecCam[i]->Render();
		}
	}

	// Level�� Stop�̳� Pause�� ���, Editor�� ī�޶� �������� ������
	else
	{
		if (m_EditorCamera != nullptr)
		{
			RenderLight(m_EditorCamera);
			GlobalCBBinding();
			m_EditorCamera->Render();
		}
	}

	// Debug Render
	RenderDebugShape();

	// Time ���� ���
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

void CRenderMgr::Blur(BLUR_STRENGTH _Strength)
{
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	Ptr<CMaterial> pBlurMtrl = nullptr;

	if (_Strength == BLUR_STRENGTH::QUARTER)
	{
		pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl1");
		m_DownScaleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DownScaleTex_1");
	}
	else if (_Strength == BLUR_STRENGTH::HALF)
	{
		pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl2");
		m_DownScaleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DownScaleTex_2");
	}
	else if (_Strength == BLUR_STRENGTH::THREE_QUARTERS)
	{
		pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl3");
		m_DownScaleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DownScaleTex_3");
	}

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

	// ����Ÿ�� ����
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
	// RenderTarget ����
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	// TargetClear
	float color[4] = { 0.f, 0.f, 0.f, 1.0f };
	CONTEXT->ClearRenderTargetView(pRTTex->GetRTV().Get(), color);
	CONTEXT->ClearDepthStencilView(pDSTex->GetDSV().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.f, 0);

	g_GlobalData.g_Resolution = Vec2((float)pRTTex->Width(), (float)pRTTex->Height());
	
	// Light2DInfo Update / Binding
	//vector<tLightInfo> vecLight2DInfo;
	//for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	//{
	//	vecLight2DInfo.push_back(m_vecLight2D[i]->GetLightInfo());
	//}
	//
	//if (vecLight2DInfo.size() == 0)
	//	vecLight2DInfo.push_back(tLightInfo{});
	//
	//// TODO : ���� ī�޶��� ��ġ�� ���ؼ� ���� �ȿ� �����ʴ� ������ ���� X
	//
	//
	//if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	//{
	//	m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	//}
	//
	//m_Light2DBuffer->SetData(vecLight2DInfo.data());
	//m_Light2DBuffer->Binding(21);
}

void CRenderMgr::RenderLight(CCamera* _Cam)
{
	//Light2DInfo Update / Binding
	vector<tLightInfo> vecLight2DInfo;

	// TODO : ���� ī�޶��� ��ġ�� ���ؼ� ���� �ȿ� �����ʴ� ������ ���� X

	for (size_t i = 0; i < m_vecLight2D.size(); ++i)
	{
		tLightInfo curLight = m_vecLight2D[i]->GetLightInfo();

		if (curLight.Type == LIGHT_TYPE::DIRECTIONAL)
		{
			vecLight2DInfo.push_back(curLight);
			continue;
		}

		// l, t, r, b
		RECT CamRound = { _Cam->Transform()->GetRelativePos().x - _Cam->Camera()->GetWidth() / 2.f
						, _Cam->Transform()->GetRelativePos().y + _Cam->Camera()->GetHeight() / 2.f
						, _Cam->Transform()->GetRelativePos().x + _Cam->Camera()->GetWidth() / 2.f
						, _Cam->Transform()->GetRelativePos().y - _Cam->Camera()->GetHeight() / 2.f };

		if (curLight.Type == LIGHT_TYPE::POINT)
		{
			Vec3 curLightLT = Vec3(curLight.WorldPos.x - (curLight.Radius / 2.f), curLight.WorldPos.y + (curLight.Radius / 2.f), 0.f);
			Vec3 curLightRB = Vec3(curLight.WorldPos.x + (curLight.Radius / 2.f), curLight.WorldPos.y - (curLight.Radius / 2.f), 0.f);

			if (curLightLT.x > CamRound.right || curLightLT.y < CamRound.bottom || 
				curLightRB.x < CamRound.left || curLightRB.y > CamRound.top)
			{
				continue;
			}
			else
			{
				vecLight2DInfo.push_back(curLight);
			}
		}
		else
		{
			vecLight2DInfo.push_back(curLight);
		}
	}

	if (vecLight2DInfo.size() == 0)
		vecLight2DInfo.push_back(tLightInfo{});

	if (m_Light2DBuffer->GetElementCount() < vecLight2DInfo.size())
	{
		m_Light2DBuffer->Create(sizeof(tLightInfo), (UINT)vecLight2DInfo.size(), SB_TYPE::SRV_ONLY, true);
	}

	g_GlobalData.g_Light2DCount = (int)vecLight2DInfo.size();
	m_Light2DBuffer->SetData(vecLight2DInfo.data());
	m_Light2DBuffer->Binding(21);
}

void CRenderMgr::GlobalCBBinding()
{
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
		// Debug ��û ��翡 �´� Mesh�� ������
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

		// ��ġ ����
		m_DebugObject->Transform()->SetWorldMatrix((*iter).matWorld);

		// ���� ����
		m_DebugObject->MeshRender()->GetMaterial()->SetScalarParam(VEC4_0, (*iter).vColor);

		if ((*iter).DepthTest)
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::LESS_EQUAL);
		else
			m_DebugObject->MeshRender()->GetMaterial()->GetShader()->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		// ������
		m_DebugObject->MeshRender()->Render();

		// ������ ���� ����� ���� ����
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
