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

	// ��ġ, ũ��, ȸ�� �������� ���ε�
	GetOwner()->Transform()->Binding();

	// ���� ���ε� (���� ���, ���̴� ���)
	if (FlipBookComponent())
		GetMaterial()->Binding();
	else
	{
		if (m_UseSpriteAsTex)
			GetMaterial()->Binding(m_ParamIdx);
		else
			GetMaterial()->Binding();
	}
		
	// ���ؽ� ����, �ε��� ���� ���ε� �� ������ ȣ��
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

