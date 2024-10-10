#include "pch.h"
#include "CCamera.h"

#include "CDevice.h"

// Manager
#include "CRenderMgr.h"
#include "CLevelMgr.h"
#include "CTimeMgr.h"
#include "CKeyMgr.h"
#include "CAssetMgr.h"

#include "CLevel.h"
#include "CLayer.h"
#include "CGameObject.h"
#include "CRenderComponent.h"

#include "CTransform.h"



CCamera::CCamera()
	: CComponent(COMPONENT_TYPE::CAMERA)
	, m_Priority(-1)
	, m_LayerCheck(0)
	, m_ProjType(PROJ_TYPE::ORTHOGRAPHIC)
	, m_Width(0)
	, m_Height(0)
	, m_Far(10000.f)
	, m_FOV(XM_PI / 2.f)
	, m_ProjectionScale(1.f)
{
	Vec2 vResolution = CDevice::GetInst()->GetResolution();
	m_Width = vResolution.x;
	m_Height = vResolution.y;
	m_AspectRatio = m_Width / m_Height;
}

CCamera::~CCamera()
{
}

void CCamera::SortGameObject()
{
	CLevel* pLevel = CLevelMgr::GetInst()->GetCurrentLevel();

	for (UINT i = 0; i < MAX_LAYER; ++i)
	{
		if (false == (m_LayerCheck & (1 << i)))
		{
			continue;
		}

		CLayer* pLayer = pLevel->GetLayer(i);

		RECT CamBound = { Transform()->GetRelativePos().x - (Camera()->GetWidth() / 2.f)
						, Transform()->GetRelativePos().y + (Camera()->GetHeight() / 2.f)
						, Transform()->GetRelativePos().x + (Camera()->GetWidth() / 2.f)
						, Transform()->GetRelativePos().y - (Camera()->GetHeight() / 2.f) };

		const vector<CGameObject*>& vecObjects = pLayer->GetObjects();
		for (size_t j = 0; j < vecObjects.size(); ++j)
		{
			if (vecObjects[j]->GetRenderComponent() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMesh() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMaterial() == nullptr
				|| vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader() == nullptr)
			{
				continue;
			}

			if (!FilterGameObject(CamBound, vecObjects[j])) continue;

			Ptr<CGraphicShader> pShader = vecObjects[j]->GetRenderComponent()->GetMaterial()->GetShader();
			SHADER_DOMAIN Domain = pShader->GetDomain();

			switch (Domain)
			{
			case DOMAIN_OPAQUE:
				m_vecOpaque.push_back(vecObjects[j]);
				break;
			case DOMAIN_MASKED:
				m_vecMasked.push_back(vecObjects[j]);
				break;
			case DOMAIN_TRANSPARENT:
				m_vecTransparent.push_back(vecObjects[j]);
				break;
			case DOMAIN_PARTICLE:
				m_vecParticles.push_back(vecObjects[j]);
				break;
			case DOMAIN_EFFECT:
				m_vecEffect.push_back(vecObjects[j]);
				break;
			case DOMAIN_POSTPROCESS:
				m_vecPostProcess.push_back(vecObjects[j]);
				break;
			case DOMAIN_UI:
				m_vecUI.push_back(vecObjects[j]);
				break;
			}
		}
	}
}

bool CCamera::FilterGameObject(RECT _CamBound, CGameObject* _Obj)
{
	if (_Obj->Transform() == nullptr) return false;

	Vec3 objPos = _Obj->Transform()->GetRelativePos();
	Vec3 objScale = _Obj->Transform()->GetRelativeScale();
	Vec3 objRot = _Obj->Transform()->GetRelativeRotation();

	Vec3 curObjLT = Vec3(objPos.x - (objScale.x / 2.f), objPos.y + (objScale.y / 2.f), 0.f);  // Left-Top
	Vec3 curObjRT = Vec3(objPos.x + (objScale.x / 2.f), objPos.y + (objScale.y / 2.f), 0.f);  // Right-Top
	Vec3 curObjLB = Vec3(objPos.x - (objScale.x / 2.f), objPos.y - (objScale.y / 2.f), 0.f);  // Left-Bottom
	Vec3 curObjRB = Vec3(objPos.x + (objScale.x / 2.f), objPos.y - (objScale.y / 2.f), 0.f);  // Right-Bottom

	if (objRot != Vec3(0.f, 0.f, 0.f))
	{
		Matrix matRot = Matrix::CreateFromYawPitchRoll(objRot.y, objRot.x, objRot.z);

		curObjLT = Vector3::Transform(curObjLT - objPos, matRot) + objPos;
		curObjRT = Vector3::Transform(curObjRT - objPos, matRot) + objPos;
		curObjLB = Vector3::Transform(curObjLB - objPos, matRot) + objPos;
		curObjRB = Vector3::Transform(curObjRB - objPos, matRot) + objPos;

		// 회전된 좌표들 중에서 가장 좌측 상단과 우측 하단 좌표를 새롭게 계산
		float minX1 = min(curObjLT.x, curObjRT.x);
		float minX2 = min(curObjLB.x, curObjRB.x);
		float minX = min(minX1, minX2);

		float minY1 = min(curObjLT.y, curObjRT.y);
		float minY2 = min(curObjLB.y, curObjRB.y);
		float minY = min(minY1, minY2);

		float maxX1 = max(curObjLT.x, curObjRT.x);
		float maxX2 = max(curObjLB.x, curObjRB.x);
		float maxX = max(maxX1, maxX2);

		float maxY1 = max(curObjLT.y, curObjRT.y);
		float maxY2 = max(curObjLB.y, curObjRB.y);
		float maxY = max(maxY1, maxY2);

		// 새로운 Left-Top과 Right-Bottom 좌표를 설정
		curObjLT = Vec3(minX, maxY, 0.f);  // Left-Top
		curObjRB = Vec3(maxX, minY, 0.f);  // Right-Bottom
	}

	if (curObjLT.x > _CamBound.right || curObjLT.y < _CamBound.bottom ||
		curObjRB.x < _CamBound.left || curObjRB.y > _CamBound.top)
	{
		return false;
	}

	return true;
}

void CCamera::Render_Effect()
{
	// 렌더타겟 변경
	Ptr<CTexture> pEffectTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectTargetTex");
	Ptr<CTexture> pEffectDepth = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectDepthStencilTex");

	// Clear
	CONTEXT->ClearRenderTargetView(pEffectTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));
	CONTEXT->ClearDepthStencilView(pEffectDepth->GetDSV().Get(), D3D11_CLEAR_STENCIL | D3D11_CLEAR_DEPTH, 1.f, 0);

	D3D11_VIEWPORT viewport = {};
	viewport.Width = pEffectTarget->Width();
	viewport.Height = pEffectTarget->Height();
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectTarget->GetRTV().GetAddressOf(), pEffectDepth->GetDSV().Get());

	// Effect
	for (size_t i = 0; i < m_vecEffect.size(); ++i)
	{
		m_vecEffect[i]->Render();
	}

	// BlurTarget 으로 변경
	Ptr<CTexture> pEffectBlurTarget = CAssetMgr::GetInst()->FindAsset<CTexture>(L"EffectBlurTargetTex");
	Ptr<CMaterial> pBlurMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"BlurMtrl2");
	Ptr<CMesh> pRectMesh = CAssetMgr::GetInst()->FindAsset<CMesh>(L"RectMesh");

	CONTEXT->ClearRenderTargetView(pEffectBlurTarget->GetRTV().Get(), Vec4(0.f, 0.f, 0.f, 0.f));

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pEffectBlurTarget->GetRTV().GetAddressOf(), nullptr);

	pBlurMtrl->SetTexParam(TEX_0, pEffectTarget);
	pBlurMtrl->Binding();
	pRectMesh->Render_Particle(2);

	// 원래 렌더타겟으로 변경
	Ptr<CTexture> pRTTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"RenderTargetTex");
	Ptr<CTexture> pDSTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"DepthStencilTex");
	Ptr<CMaterial> pEffectMergeMtrl = CAssetMgr::GetInst()->FindAsset<CMaterial>(L"EffectMergeMtrl");

	viewport.Width = pRTTex->Width();
	viewport.Height = pRTTex->Height();
	viewport.MinDepth = 0.f;
	viewport.MaxDepth = 1.f;

	CONTEXT->RSSetViewports(1, &viewport);
	CONTEXT->OMSetRenderTargets(1, pRTTex->GetRTV().GetAddressOf(), pDSTex->GetDSV().Get());

	pEffectMergeMtrl->SetTexParam(TEX_0, pEffectTarget);
	pEffectMergeMtrl->SetTexParam(TEX_1, pEffectBlurTarget);
	pEffectMergeMtrl->Binding();
	pRectMesh->Render();
}

void CCamera::Begin()
{
	if (-1 != m_Priority)
	{
		CRenderMgr::GetInst()->RegisterCamera(this, m_Priority);
	}
}

void CCamera::FinalTick()
{
	// View 행렬 계산
	// 이동 행렬
	Matrix matTrans = XMMatrixTranslation(-Transform()->GetRelativePos().x
										, -Transform()->GetRelativePos().y
										, -Transform()->GetRelativePos().z);

	// 회전 행렬
	Matrix matRot;
	Vec3 vR = Transform()->GetWorldDir(DIR::RIGHT);
	Vec3 vU = Transform()->GetWorldDir(DIR::UP);
	Vec3 vF = Transform()->GetWorldDir(DIR::FRONT);

	// 방향 벡터의 역행렬 (전치 행렬)
	matRot._11 = vR.x; matRot._12 = vU.x; matRot._13 = vF.x;
	matRot._21 = vR.y; matRot._22 = vU.y; matRot._23 = vF.y;
	matRot._31 = vR.z; matRot._32 = vU.z; matRot._33 = vF.z;

	m_matView = matTrans * matRot;


	// Projection Space, 투영 좌표계
	if (PROJ_TYPE::ORTHOGRAPHIC == m_ProjType)
	{
		// 직교 투영
		m_matProj = XMMatrixOrthographicLH(m_Width * m_ProjectionScale, m_Height * m_ProjectionScale, 1.f, m_Far);
	}
	else
	{
		// 원근 투영
		m_matProj = XMMatrixPerspectiveFovLH(m_FOV, m_AspectRatio, 1.f, m_Far);
	}
}

void CCamera::SaveToFile(FILE* _File)
{
	fwrite(&m_Priority, sizeof(int), 1, _File);
	fwrite(&m_LayerCheck, sizeof(UINT), 1, _File);
	fwrite(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fwrite(&m_Width, sizeof(float), 1, _File);
	fwrite(&m_Height, sizeof(float), 1, _File);
	fwrite(&m_AspectRatio, sizeof(float), 1, _File);
	fwrite(&m_Far, sizeof(float), 1, _File);
	fwrite(&m_FOV, sizeof(float), 1, _File);
	fwrite(&m_ProjectionScale, sizeof(float), 1, _File);
}

void CCamera::LoadFromFile(FILE* _File)
{
	fread(&m_Priority, sizeof(int), 1, _File);
	fread(&m_LayerCheck, sizeof(UINT), 1, _File);
	fread(&m_ProjType, sizeof(PROJ_TYPE), 1, _File);
	fread(&m_Width, sizeof(float), 1, _File);
	fread(&m_Height, sizeof(float), 1, _File);
	fread(&m_AspectRatio, sizeof(float), 1, _File);
	fread(&m_Far, sizeof(float), 1, _File);
	fread(&m_FOV, sizeof(float), 1, _File);
	fread(&m_ProjectionScale, sizeof(float), 1, _File);
}

void CCamera::Render() 
{
	// 오브젝트 분류
	SortGameObject();

	// 물체가 렌더링될 때 사용할 View, Proj 행렬
	g_Trans.matView = m_matView;
	g_Trans.matProj = m_matProj;

	// Opaque
	for (size_t i = 0; i < m_vecOpaque.size(); ++i)
	{
		m_vecOpaque[i]->Render();
	}

	// Masked
	for (size_t i = 0; i < m_vecMasked.size(); ++i)
	{
		m_vecMasked[i]->Render();
	}

	// Transparent
	for (size_t i = 0; i < m_vecTransparent.size(); ++i)
	{
  		m_vecTransparent[i]->Render();

		int index = m_vecTransparent[i]->GetLayerIdx();
		if (1 < index && index < 5)
		{
			switch (index)
			{
			case 2:         // LAYER_TYPE::BACKGROUND3;
				CRenderMgr::GetInst()->Blur(BLUR_STRENGTH::QUARTER);
				break;
			case 3:         // LAYER_TYPE::BACKGROUND2;
				CRenderMgr::GetInst()->Blur(BLUR_STRENGTH::HALF);
				break;
			case 4:
				CRenderMgr::GetInst()->Blur(BLUR_STRENGTH::THREE_QUARTERS);
				break;
			}
		}
	}

	// Particles
	for (size_t i = 0; i < m_vecParticles.size(); ++i)
	{
		m_vecParticles[i]->Render();
	}

	Render_Effect();

	// PostProcess
	for (size_t i = 0; i < m_vecPostProcess.size(); ++i)
	{
		CRenderMgr::GetInst()->PostProcessCopy();
		m_vecPostProcess[i]->Render();
	}

	// UI
	for (size_t i = 0; i < m_vecUI.size(); ++i)
	{
		m_vecUI[i]->Render();
	}

	m_vecOpaque.clear();
	m_vecMasked.clear();
	m_vecTransparent.clear();
	m_vecEffect.clear();
	m_vecParticles.clear();
	m_vecPostProcess.clear();
	m_vecUI.clear();
}

