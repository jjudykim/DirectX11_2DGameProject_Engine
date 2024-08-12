#include "pch.h"
#include "CFlipBookComponent.h"

#include "CTimeMgr.h"
#include "CDevice.h"
#include "CConstBuffer.h"


CFlipBookComponent::CFlipBookComponent()
	: CComponent(COMPONENT_TYPE::FLIPBOOKCOMPONENT)
	, m_CurFlipBook(nullptr)
	, m_CurFrmIdx(0)
{
}

CFlipBookComponent::CFlipBookComponent(CFlipBookComponent& _Origin)
	: CComponent(_Origin)
	, m_vecFlipBook(_Origin.m_vecFlipBook)
	, m_CurFlipBook(_Origin.m_CurFlipBook)
	, m_CurFrmIdx(0)
	, m_FPS(_Origin.m_FPS)
	, m_AccTime(0.f)
	, m_Repeat(_Origin.m_Repeat)
	, m_Finish(false)
{
	if (m_CurFlipBook != nullptr)
	{
		int FlipBookIdx = 0;
		for (; FlipBookIdx < (int)m_vecFlipBook.size(); ++FlipBookIdx)
		{
			if (m_CurFlipBook == m_vecFlipBook[FlipBookIdx])
				break;
		}
		Play(FlipBookIdx, m_FPS, m_Repeat);
	}
}

CFlipBookComponent::~CFlipBookComponent()
{
}

void CFlipBookComponent::FinalTick()
{
	if (m_Finish)
	{
		if (m_Repeat == false)
			return;

		Reset();
	}

	if (m_CurFlipBook != nullptr)
	{
		float MaxTime = 1.f / m_FPS;

		m_AccTime += DT;

		if (MaxTime < m_AccTime)
		{
			m_AccTime -= MaxTime;
			++m_CurFrmIdx;

			if (m_CurFlipBook->GetMaxFrameCount() <= m_CurFrmIdx)
			{
				--m_CurFrmIdx;
				m_Finish = true;
			}
		}

		m_CurFrmSprite = m_CurFlipBook->GetSprite(m_CurFrmIdx);
	}
}

void CFlipBookComponent::SaveToFile(FILE* _File)
{
	// FlipBook Count / Info
	size_t FlipBookCount = m_vecFlipBook.size();
	fwrite(&FlipBookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < m_vecFlipBook.size(); ++i)
	{
		SaveAssetRef(m_vecFlipBook[i], _File);
	}

	// Currenet FlipBook Info
	SaveAssetRef(m_CurFlipBook, _File);

	// Current Sprite Info
	SaveAssetRef(m_CurFrmSprite, _File);

	// Current Frame Index / FPS / Time / Repeat
	fwrite(&m_CurFrmIdx, sizeof(int), 1, _File);
	fwrite(&m_FPS, sizeof(float), 1, _File);
	fwrite(&m_AccTime, sizeof(float), 1, _File);
	fwrite(&m_Repeat, sizeof(bool), 1, _File);
}

void CFlipBookComponent::LoadFromFile(FILE* _File)
{
	// FlipBook Count / Info
	size_t FlipBookCount = 0;
	fread(&FlipBookCount, sizeof(size_t), 1, _File);
	for (size_t i = 0; i < FlipBookCount; ++i)
	{
		Ptr<CFlipBook> pFlipBook;
		LoadAssetRef(pFlipBook, _File);
		m_vecFlipBook.push_back(pFlipBook);
	}

	// Currenet FlipBook Info
	LoadAssetRef(m_CurFlipBook, _File);
	
	// Current Sprite Info
	LoadAssetRef(m_CurFrmSprite, _File);

	// Current Frame Index / FPS / Time / Repeat
	fread(&m_CurFrmIdx, sizeof(int), 1, _File);
	fread(&m_FPS, sizeof(float), 1, _File);
	fread(&m_AccTime, sizeof(float), 1, _File);
	fread(&m_Repeat, sizeof(bool), 1, _File);
}

void CFlipBookComponent::AddFlipBook(int _Idx, Ptr<CFlipBook> _FlipBook)
{
	if (m_vecFlipBook.size() <= _Idx)
	{
		m_vecFlipBook.resize(_Idx + 1);
	}

	m_vecFlipBook[_Idx] = _FlipBook;
}

Ptr<CFlipBook> CFlipBookComponent::FindFlipBook(const wstring& _Key)
{
	for (size_t i = 0; i < m_vecFlipBook.size(); ++i)
	{
		if (m_vecFlipBook[i]->GetKey() == _Key)
			return m_vecFlipBook[i];
	}

	return nullptr;
}

void CFlipBookComponent::Play(int _FlipBookIdx, float _FPS, bool _Repeat)
{
	m_CurFlipBook = m_vecFlipBook[_FlipBookIdx];

	if (m_CurFlipBook == nullptr)
	{
		return;
	}

	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_FPS = _FPS;
	m_Repeat = _Repeat;
}

void CFlipBookComponent::Reset()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0.f;
	m_Finish = false;
}

void CFlipBookComponent::Binding()
{
	if (m_CurFrmSprite != nullptr)
	{
		// 현재 표시해야하는 Sprite의 정보를, Sprite 전용 상수버퍼를 통해 GPU에 전달
		tSpriteInfo tInfo = {};

		tInfo.LeftTopUV     = m_CurFrmSprite->GetLeftTopUV();
		tInfo.SliceUV       = m_CurFrmSprite->GetSliceUV();
		tInfo.BackGroundUV  = m_CurFrmSprite->GetBackgroundUV();
		tInfo.OffsetUV      = m_CurFrmSprite->GetOffsetUV();
		tInfo.UseFlipBook   = 1;

		static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);

		CB->SetData(&tInfo);
		CB->Binding();

		// FlipBook Sprite 아틀라스 텍스쳐 전용 레지스터 번호 t10에 바인딩
		Ptr<CTexture> pAtlas = m_CurFrmSprite->GetAtlasTexture();
		pAtlas->Binding(10);
	}
	else
	{
		Clear();
	}
}

void CFlipBookComponent::Clear()
{
	tSpriteInfo tInfo = {};
	static CConstBuffer* CB = CDevice::GetInst()->GetConstBuffer(CB_TYPE::SPRITE);
	CB->SetData(&tInfo);
	CB->Binding();
}
