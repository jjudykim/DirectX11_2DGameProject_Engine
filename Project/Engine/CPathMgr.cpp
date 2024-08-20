#include "pch.h"
#include "CPathMgr.h"

#include "CEngine.h"

CPathMgr::CPathMgr()
{
}

CPathMgr::~CPathMgr()
{
}

void CPathMgr::Init()
{
	// 실행 경로 얻어내기
	wchar_t szBuffer[256] = {};
	//GetCurrentDirectory(256, szBuffer);
	GetModuleFileName(NULL, szBuffer, 256);
	
	GetParentPath(szBuffer);
	GetParentPath(szBuffer);

	m_Content = szBuffer;
	m_Content += L"\\content\\";
}

void CPathMgr::Render()
{
}

void CPathMgr::GetParentPath(wchar_t* _Buffer)
{
	size_t len = wcslen(_Buffer);

	for (size_t i = len - 1; 0 < i; --i)
	{
		if (L'\\' == _Buffer[i])
		{
			_Buffer[i] = L'\0';
			break;
		}
	}
}

wstring CPathMgr::GetRelativePath(const wstring& _FilePath)
{
	size_t FindPos = _FilePath.find(m_Content);
	if (FindPos == wstring::npos)
		return L"";

	return _FilePath.substr(FindPos + m_Content.length(), _FilePath.length());
}

wstring CPathMgr::GetParentPath(const wstring& _FilePath)
{
	wchar_t* Buffer = (wchar_t*)_FilePath.c_str();
	size_t len = wcslen(Buffer);

	for (size_t i = len - 1; 0 < i; --i)
	{
		if (L'\\' == Buffer[i])
		{
			Buffer[i] = L'\0';
			break;
		}
	}

	return wstring(Buffer);
}
