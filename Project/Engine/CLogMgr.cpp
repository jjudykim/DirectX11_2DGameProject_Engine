#include "pch.h"

#include "CLogMgr.h"
#include "CFontMgr.h"


CLogMgr::CLogMgr()
{
}

CLogMgr::~CLogMgr()
{

}

void CLogMgr::Init()
{
}

void CLogMgr::Render()
{
	for (size_t i = 0; i < m_vecLog.size(); ++i)
	{
		LOG_INFO curInfo = m_vecLog[i];

		CFontMgr::GetInst()->DrawFont(curInfo.m_LogContent
									, curInfo.m_xPos, curInfo.m_yPos, curInfo.m_Size
									, FONT_RGBA(curInfo.m_Color.x, curInfo.m_Color.y, curInfo.m_Color.z, curInfo.m_Color.w));
	}

	m_vecLog.clear();
}
