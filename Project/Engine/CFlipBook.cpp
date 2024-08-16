#include "pch.h"
#include "CFlipBook.h"

#include "CAssetMgr.h"

CFlipBook::CFlipBook()
	: CAsset(ASSET_TYPE::FLIPBOOK)
	, m_FPS(0)
{
}

CFlipBook::~CFlipBook()
{
}

void CFlipBook::FinalTick()
{
}

int CFlipBook::Save(const wstring& _FilePath)
{
	// FlipBook Save
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"wb");

	if (File == nullptr)
		return E_FAIL;
	
	fwrite(&m_FPS, sizeof(float), 1, File);

	size_t SpriteCount = m_vecSprite.size();
	fwrite(&SpriteCount, sizeof(size_t), 1, File);

	for (size_t i = 0; i < SpriteCount; ++i)
	{
		SaveAssetRef(m_vecSprite[i], File);
	}

	fclose(File);

	return S_OK;
}

int CFlipBook::Load(const wstring& _FilePath)
{
	// FlipBook Load
	FILE* File = nullptr;
	_wfopen_s(&File, _FilePath.c_str(), L"rb");

	if (File == nullptr)
		return E_FAIL;

	fread(&m_FPS, sizeof(float), 1, File);

	size_t SpriteCount = 0;
	fread(&SpriteCount, sizeof(size_t), 1, File);
	m_vecSprite.resize(SpriteCount);

	for (size_t i = 0; i < SpriteCount; ++i)
	{
		LoadAssetRef(m_vecSprite[i], File);
	}

	fclose(File);

	return S_OK;
}