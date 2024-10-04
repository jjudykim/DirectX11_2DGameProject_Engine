#include "pch.h"
#include "CFontMgr.h"

#include "CDevice.h"

CFontMgr::CFontMgr()
	: m_FW1Factory(nullptr)
	, m_LogFontWrapper(nullptr)
	, m_UIFontWrapper(nullptr)
{
}

CFontMgr::~CFontMgr()
{
	if (m_FW1Factory != nullptr)
		m_FW1Factory->Release();

	if (m_LogFontWrapper != nullptr)
		m_LogFontWrapper->Release();

	if (m_UIFontWrapper != nullptr)
		m_UIFontWrapper->Release();
}

void CFontMgr::Init()
{
	if (FAILED(FW1CreateFactory(FW1_VERSION, &m_FW1Factory)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"Consolas", &m_LogFontWrapper)))
	{
		assert(nullptr);
	}

	if (FAILED(m_FW1Factory->CreateFontWrapper(DEVICE, L"CookieRunOTF", &m_UIFontWrapper)))
	{
		assert(nullptr);
	}
}

void CFontMgr::DrawFontForLog(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_LogFontWrapper->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// X position
		_fPosY,				// Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}

void CFontMgr::DrawFontForUI(const wchar_t* _pStr, float _fPosX, float _fPosY, float _fFontSize, UINT _Color)
{
	m_UIFontWrapper->DrawString(
		CONTEXT,
		_pStr,				// String
		_fFontSize,			// Font size
		_fPosX,				// X position
		_fPosY,				// Y position
		_Color,				// Text color, 0xAaBbGgRr
		FW1_RESTORESTATE    // Flags (for example FW1_RESTORESTATE to keep context states unchanged)
	);
}
