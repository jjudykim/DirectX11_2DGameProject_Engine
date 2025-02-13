#pragma once
#include "singleton.h"
class CPathMgr :
    public CSingleton<CPathMgr>
{
    SINGLE(CPathMgr);

private:
    wstring     m_Content;
    wstring     m_Solution;

public:
    void Init();
    void Render();

private:
    void GetParentPath(wchar_t* _Buffer);

public:
    const wstring& GetContentPath() { return m_Content; }
    const wstring& GetSolutionPath() { return m_Solution; }
    wstring GetRelativePath(const wstring& _FilePath);
    wstring GetParentPath(const wstring& _FilePath);
};

