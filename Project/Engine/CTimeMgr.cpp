#include "pch.h"
#include "CTimeMgr.h"

#include "CKeyMgr.h"

#include "CEngine.h"
#include "CLevelMgr.h"
#include "CFontMgr.h"
#include "CLevel.h"

CTimeMgr::CTimeMgr()
	: m_llCurCount{}
	, m_llPrevCount{}
	, m_llFrequency{}
	, m_FPS(0)
	, m_DeltaTime(0.f)
	, m_Time(0.f)
	, m_E_DeltaTime(0.f)
	, m_E_Time(0.f)
	, m_TimeInfo{}
{
}

CTimeMgr::~CTimeMgr()
{
}

void CTimeMgr::Init()
{
	QueryPerformanceFrequency(&m_llFrequency);

	QueryPerformanceCounter(&m_llCurCount);
	m_llPrevCount = m_llCurCount;
}

void CTimeMgr::Tick()
{
	QueryPerformanceCounter(&m_llCurCount);

	m_E_DeltaTime = (float)(m_llCurCount.QuadPart - m_llPrevCount.QuadPart) / (float)m_llFrequency.QuadPart;

	// DT º¸Á¤
	if (1.f / 60.f < m_E_DeltaTime)
		m_E_DeltaTime = 1.f / 60.f;

	m_E_Time += m_E_DeltaTime;
	m_llPrevCount = m_llCurCount;

	++m_FPS;

	static float AccTime = 0.f;
	AccTime += m_E_DeltaTime;

	if (1.f < AccTime)
	{
		swprintf_s(m_TimeInfo, L"Engine_DeltaTime : %f, FPS : %d", m_E_DeltaTime, m_FPS);
		//SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);
		AccTime = 0.f;
		m_FPS = 0;
	}

	// Level¿ë DT
	CLevel* pCurLevel = CLevelMgr::GetInst()->GetCurrentLevel();
	if (pCurLevel == nullptr || pCurLevel->GetState() != LEVEL_STATE::PLAY)
	{
		m_DeltaTime = 0.f;
	}
	else
	{
		m_DeltaTime = m_E_DeltaTime;
	}
	
	m_Time += m_DeltaTime;

	// Global Data
	g_GlobalData.g_DT = m_DeltaTime;
	g_GlobalData.g_EngineDT = m_E_DeltaTime;
	g_GlobalData.g_Time = m_Time;
	g_GlobalData.g_EngineTime = m_E_Time;

	TimerUpdate();
}

void CTimeMgr::TimerUpdate()
{
	for (size_t i = 0; i < m_VecTimers.size(); ++i)
	{
		m_VecTimers[i].CurrentTime += DT;
		if (m_VecTimers[i].Duration <= m_VecTimers[i].CurrentTime)
		{
			m_VecTimers[i].Callback();
			if (m_VecTimers[i].Repeat)
			{
				m_VecTimers[i].CurrentTime = 0.0f;
			}
			else
			{
				m_VecTimers.erase(m_VecTimers.begin() + i);
				continue;
			}
		}
	}
}

void CTimeMgr::Render()
{
	CFontMgr::GetInst()->DrawFontForLog(m_TimeInfo, 10, 20, 16, FONT_RGBA(153, 187, 255, 255));
}