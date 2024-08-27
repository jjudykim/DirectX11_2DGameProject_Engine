#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial(bool _IsEngine)
	: CAsset(ASSET_TYPE::MATERIAL)
	, m_UseSpriteAsTex(false)
{
	if (_IsEngine)
	{
		SetEngineAsset();
	}
}

CMaterial::~CMaterial()
{
}

void* CMaterial::GetScalarParam(SCALAR_PARAM _Param)
{
	switch (_Param)
	{
	case INT_0:
	case INT_1:
	case INT_2:
	case INT_3:
		return m_Const.iArr + _Param;
		break;
	case FLOAT_0:
	case FLOAT_1:
	case FLOAT_2:
	case FLOAT_3:
		return m_Const.fArr + (_Param - FLOAT_0);
		break;
	case VEC2_0:
	case VEC2_1:
	case VEC2_2:
	case VEC2_3:
		return m_Const.v2Arr + (_Param - VEC2_0);
		break;

	case VEC4_0:
	case VEC4_1:
	case VEC4_2:
	case VEC4_3:
		return m_Const.v4Arr + (_Param - VEC4_0);
		break;

	case MAT_0:
	case MAT_1:
	case MAT_2:
	case MAT_3:
		return m_Const.matArr + (_Param - MAT_0);
		break;
	}

	return nullptr;
}

void CMaterial::Binding(int _index)
{
	if (!m_Shader)
		return;

	m_Const.iArr[0] = _index;

	for (int i = 0; i < TEX_PARAM::END; ++i)
	{
		if (m_arrSprite[i] == nullptr)
		{
			m_Const.btex[i] = 0;
			CTexture::Clear(i);
			continue;
		}

		m_Const.btex[i] = 1;

		if (i == _index)
		{
			m_arrSprite[i]->BindingAtTexRegister(i);
		}
	}

	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding();

	m_Shader->Binding();
}

void CMaterial::Binding()
{
	if (!m_Shader)
		return;

	for (int i = 0; i < TEX_PARAM::END; ++i)
	{
		if (m_arrTex[i] == nullptr)
		{
			m_Const.btex[i] = 0;
			CTexture::Clear(i);
			continue;
		}

		m_Const.btex[i] = 1;
		m_arrTex[i]->Binding(i);
	}

	CConstBuffer* pCB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::MATERIAL);
	pCB->SetData(&m_Const);
	pCB->Binding();

	m_Shader->Binding();
}

int CMaterial::Save(const wstring& _RelativePath)
{
	SetRelativePath(_RelativePath);

	wstring strFilePath = CPathMgr::GetInst()->GetContentPath();
	strFilePath += _RelativePath;

	FILE* File = nullptr;
	_wfopen_s(&File, strFilePath.c_str(), L"wb");

	if (File == nullptr)
		return E_FAIL;

	// ���̴� ���� ����
	SaveAssetRef(m_Shader, File);

	// ��� ������ ����
	fwrite(&m_Const, sizeof(tMtrlConst), 1, File);

	// ��������Ʈ Ȱ�� ����
	fwrite(&m_UseSpriteAsTex, sizeof(bool), 1, File);

	// ���� �ؽ��� ����
	if (m_UseSpriteAsTex)
	{
		for(UINT i = 0; i < TEX_PARAM::END; ++i)
			SaveAssetRef(m_arrSprite[i], File);
	}
	else
	{
		for (UINT i = 0; i < TEX_PARAM::END; ++i)
			SaveAssetRef(m_arrTex[i], File);
	}
	
	fclose(File);

	return S_OK;
}

int CMaterial::Load(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	if (File == nullptr)
		return E_FAIL;

	// ���̴� ���� ����
	LoadAssetRef(m_Shader, File);

	// ��� ������ ����
	fread(&m_Const, sizeof(tMtrlConst), 1, File);

	// ��������Ʈ Ȱ�� ����
	fread(&m_UseSpriteAsTex, sizeof(bool), 1, File);

	if (m_UseSpriteAsTex)
	{
		for(UINT i = 0; i < TEX_PARAM::END; ++i)
			LoadAssetRef(m_arrSprite[i], File);
	}
	else
	{
		for (UINT i = 0; i < TEX_PARAM::END; ++i)
		{
			LoadAssetRef(m_arrTex[i], File);
		}
	}
	
	fclose(File);

	return S_OK;
}