#include "pch.h"
#include "CMeshRender.h"

#include "CTransform.h"
#include "CFlipBookComponent.h"

CMeshRender::CMeshRender()
	: CRenderComponent(COMPONENT_TYPE::MESHRENDER)
{
}

CMeshRender::~CMeshRender()
{
}


void CMeshRender::FinalTick()
{
}

void CMeshRender::Render()
{
	if (!GetMesh() || !GetMaterial() || !GetMaterial()->GetShader())
		return;

	if (FlipBookComponent())
		FlipBookComponent()->Binding();
	else
		CFlipBookComponent::Clear();

	// 위치, 크기, 회전 상태정보 바인딩
	GetOwner()->Transform()->Binding();

	// 재질 바인딩 (재질 상수, 쉐이더 등등)
	if (FlipBookComponent())
		GetMaterial()->Binding();
	else
	{
		if (m_UseSpriteAsTex)
			GetMaterial()->Binding(m_ParamIdx);
		else
			GetMaterial()->Binding();
	}
		
	// 버텍스 버퍼, 인덱스 버퍼 바인딩 및 렌더링 호출
	GetMesh()->Render();
}

void CMeshRender::SaveToFile(FILE* _File)
{
	fwrite(&m_UseSpriteAsTex, sizeof(bool), 1, _File);
	fwrite(&m_ParamIdx, sizeof(int), 1, _File);
	SaveDataToFile(_File);
}

void CMeshRender::LoadFromFile(FILE* _File)
{
	fread(&m_UseSpriteAsTex, sizeof(bool), 1, _File);
	fread(&m_ParamIdx, sizeof(int), 1, _File);
	LoadDataFromFile(_File);
}

