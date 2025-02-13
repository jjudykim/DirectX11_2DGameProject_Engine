#include "pch.h"
#include "CMaterial.h"

#include "CDevice.h"
#include "CConstBuffer.h"

CMaterial::CMaterial(bool _IsEngine)
	: CAsset(ASSET_TYPE::MATERIAL)
	, m_UseSpriteAsTex(false)
	, m_EffectElapsedTime(0)
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

	if (m_UseDeadEffect)
	{
		m_Const.iArr[2] = 1;
		m_Const.fArr[2] = m_EffectElapsedTime;
		m_Shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		m_EffectElapsedTime += DT;
	}
	else
	{
		m_Const.iArr[2] = 0;
		m_Const.fArr[2] = 0;
		m_EffectElapsedTime = 0.f;
	}
	
	if (m_UseBlinkEffect)
	{
		m_Const.iArr[3] = 1;
		m_Shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	}
	else
	{
		m_Const.iArr[3] = 0;
		m_Shader->SetDSType(DS_TYPE::LESS_EQUAL);
	}

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

	m_Const.iArr[0] = -1;

	if (m_UseDeadEffect)
	{
		m_Const.iArr[2] = 1;
		m_Const.fArr[2] = m_EffectElapsedTime;
		m_Shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);

		m_EffectElapsedTime += DT;
	}
	else
	{
		m_Const.iArr[2] = 0;
		m_Const.fArr[2] = 0;
		m_EffectElapsedTime = 0;
	}

	if (m_UseBlinkEffect)
	{
		m_Const.iArr[3] = 1;
		m_Shader->SetDSType(DS_TYPE::NO_TEST_NO_WRITE);
	}
	else
	{
		m_Const.iArr[3] = 0;
		m_Shader->SetDSType(DS_TYPE::LESS_EQUAL);
	}
		
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

	// 쉐이더 참조 정보
	SaveAssetRef(m_Shader, File);
	
	//if (m_Shader != nullptr)
	//{
	//	vector<tScalarParam> vScalar = m_Shader->GetScalarParam();
	//	int scalarSize = vScalar.size();
	//	fwrite(&scalarSize, sizeof(size_t), 1, File);
	//	for (size_t i = 0; i < vScalar.size(); ++i)
	//	{
	//		tScalarParam tScalar = vScalar[i];
	//		fwrite(&tScalar.ParamType, sizeof(SCALAR_PARAM), 1, File);
	//		SaveStringToFile(tScalar.strDesc, File);
	//	}
	//
	//	vector<tTexParam> vTex = m_Shader->GetTexParam();
	//	int texSize = vTex.size();
	//	fwrite(&texSize, sizeof(size_t), 1, File);
	//	for (size_t i = 0; i < vTex.size(); ++i)
	//	{
	//		tTexParam tTex = vTex[i];
	//		fwrite(&tTex.ParamType, sizeof(TEX_PARAM), 1, File);
	//		SaveStringToFile(tTex.strDesc, File);
	//
	//	}
	//}
	
	// 상수 데이터 정보
	fwrite(&m_Const, sizeof(tMtrlConst), 1, File);

	// 스프라이트 활용 정보
	fwrite(&m_UseSpriteAsTex, sizeof(bool), 1, File);

	// 참조 텍스쳐 정보
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

	// 쉐이더 참조 정보
	LoadAssetRef(m_Shader, File);

	//if (m_Shader != nullptr)
	//{
	//	int scalarSize = 0;
	//	fread(&scalarSize, sizeof(size_t), 1, File);
	//	for (size_t i = 0; i < scalarSize; ++i)
	//	{
	//		tScalarParam tScalar;
	//		fread(&tScalar, sizeof(tScalarParam), 1, File);
	//		//m_Shader->AddScalarParam(tScalar.ParamType, tScalar.strDesc);
	//	}
	//
	//	int texSize = 0;
	//	fread(&texSize, sizeof(size_t), 1, File);
	//	for (size_t i = 0; i < texSize; ++i)
	//	{
	//		tTexParam tTex;
	//		fread(&tTex, sizeof(tTexParam), 1, File);
	//		//m_Shader->AddTexParam(tTex.ParamType, tTex.strDesc);
	//	}
	//}

	// 상수 데이터 정보
	fread(&m_Const, sizeof(tMtrlConst), 1, File);

	// 스프라이트 활용 정보
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