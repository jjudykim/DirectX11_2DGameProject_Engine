#include "pch.h"
#include "CSprite.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CTexture.h"
#include "CConstBuffer.h"

CSprite::CSprite()
	: CAsset(ASSET_TYPE::SPRITE)
{
}

CSprite::~CSprite()
{
}

void CSprite::SetLeftTop(Vec2 _LeftTop)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_LeftTopUV = _LeftTop / AtlasResolution;
}

void CSprite::SetSlice(Vec2 _Slice)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_SliceUV = _Slice / AtlasResolution;
}

void CSprite::SetBackground(Vec2 _Background)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_BackgroundUV = _Background / AtlasResolution;
}

void CSprite::SetOffset(Vec2 _Offset)
{
	Vec2 AtlasResolution = Vec2((float)m_Atlas->Width(), (float)m_Atlas->Height());
	m_OffsetUV = _Offset / AtlasResolution;
}

void CSprite::Binding()
{
	tSpriteInfo tInfo = {};

	if (this == nullptr)
	{
		Clear();
		return;
	}

	tInfo.LeftTopUV     = m_LeftTopUV;
	tInfo.SliceUV       = m_SliceUV;
	tInfo.BackGroundUV  = m_BackgroundUV;
	tInfo.OffsetUV      = m_OffsetUV;
	tInfo.UseFlipBook   = 1;

	static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

	CB->SetData(&tInfo);
	CB->Binding();

	// FlipBook Sprite 아틀라스 텍스쳐 전용 레지스터 번호 t10에 바인딩
	m_Atlas->Binding(20);
}

void CSprite::BindingAtTexRegister(UINT _RegisterNum)
{
	tSpriteInfo tInfo = {};

	if (this == nullptr)
	{
		Clear();
		return;
	}

	tInfo.LeftTopUV = m_LeftTopUV;
	tInfo.SliceUV = m_SliceUV;
	tInfo.BackGroundUV = m_BackgroundUV;
	tInfo.OffsetUV = m_OffsetUV;
	tInfo.UseFlipBook = 1;

	static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

	CB->SetData(&tInfo);
	CB->Binding();

	// FlipBook Sprite 아틀라스 텍스쳐 전용 레지스터 번호 t10에 바인딩
	m_Atlas->Binding(_RegisterNum);
}

void CSprite::Clear()
{
	tSpriteInfo tInfo = {};
	static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);
	CB->SetData(&tInfo);
	CB->Binding();
}

void CSprite::Create(Ptr<CTexture> _Atlas, Vec2 _LeftTopPixel, Vec2 _SlicePixel)
{
	m_Atlas = _Atlas;
	assert(m_Atlas);

	UINT width = m_Atlas->Width();
	UINT height = m_Atlas->Height();

	m_LeftTopUV = Vec2(_LeftTopPixel.x / (float)width, _LeftTopPixel.y / (float)height);
	m_SliceUV = Vec2(_SlicePixel.x / (float)width, _SlicePixel.y / (float)height);
}

int CSprite::Save(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	if (File == nullptr)
		return E_FAIL;

	// Atlas 참조정보 저장
	SaveAssetRef(m_Atlas, File);

	fwrite(&m_LeftTopUV, sizeof(Vec2), 1, File);
	fwrite(&m_SliceUV, sizeof(Vec2), 1, File);
	fwrite(&m_BackgroundUV, sizeof(Vec2), 1, File);
	fwrite(&m_OffsetUV, sizeof(Vec2), 1, File);

	// Bundle Name
	size_t length = m_BundleName.size();
	fwrite(&length, sizeof(length), 1, File);

	fwrite(m_BundleName.c_str(), sizeof(wchar_t), length, File);

	fclose(File);

	return S_OK;
}

int CSprite::Load(const wstring& _FilePath)
{
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	if (File == nullptr)
	{
		return E_FAIL;
	}

	// Atlas 키, 경로 불러오기
	LoadAssetRef(m_Atlas, File);

	fread(&m_LeftTopUV, sizeof(Vec2), 1, File);
	fread(&m_SliceUV, sizeof(Vec2), 1, File);
	fread(&m_BackgroundUV, sizeof(Vec2), 1, File);
	fread(&m_OffsetUV, sizeof(Vec2), 1, File);

	// Bundle Name
	size_t length;
	std::fread(&length, sizeof(length), 1, File);
	
	wstring bundleName(length, L'\0');
	fread(&bundleName[0], sizeof(wchar_t), length, File);
	m_BundleName = bundleName;

	fclose(File);

	return S_OK;
}