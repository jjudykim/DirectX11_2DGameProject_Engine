#include "pch.h"
#include "CTimeMgr.h"

#include "CEngine.h"
#include "CLevelMgr.h"
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
		wchar_t szBuff[255] = {};
		swprintf_s(szBuff, L"Engine_DeltaTime : %f, FPS : %d", m_E_DeltaTime, m_FPS);
		SetWindowText(CEngine::GetInst()->GetMainWnd(), szBuff);
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
}