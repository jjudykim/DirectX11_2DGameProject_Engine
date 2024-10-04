#pragma once

#include "singleton.h"

struct LOG_INFO
{
	const wchar_t*    m_LogContent;
	float             m_xPos;
	float             m_yPos;
	float             m_Size;
	Vec4              m_Color;
};

class CLogMgr
	: public CSingleton<CLogMgr>
{
	SINGLE(CLogMgr);

private:
	vector<LOG_INFO>        m_vecLog;
	vector<LOG_INFO>        m_vecUI;

public:
	void AddLog(LOG_INFO _newLog) { m_vecLog.push_back(_newLog); }
	void AddUI(LOG_INFO _newLog) { m_vecUI.push_back(_newLog); }
	void Init();
	void Render();
};

