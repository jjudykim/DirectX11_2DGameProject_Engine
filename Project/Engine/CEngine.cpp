#include "pch.h"
#include "CEngine.h"

#include "CDevice.h"

// Manager
#include "CPathMgr.h"
#include "CKeyMgr.h"
#include "CTimeMgr.h"
#include "CAssetMgr.h"
#include "CLevelMgr.h"
#include "CRenderMgr.h"
#include "CCollisionMgr.h"
#include "CFontMgr.h"
#include "CTaskMgr.h"
#include "CLogMgr.h"

#include "CPrefab.h"

CEngine::CEngine()
	: m_hWnd(nullptr)
	, m_ptResolution{}
{
}

CEngine::~CEngine()
{
	if (m_FMODSystem != nullptr)
	{
		m_FMODSystem->release();
		m_FMODSystem = nullptr;
	}
}

int CEngine::Init(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _LoadFunc)
{
	m_hWnd = _wnd;
	m_ptResolution = _ptResolution;
	ChangeWindowScale(_ptResolution.x, _ptResolution.y);

	if (FAILED(CDevice::GetInst()->Init(m_hWnd, m_ptResolution.x, m_ptResolution.y)))
	{
		MessageBox(nullptr, L"��ġ �ʱ�ȭ ����", L"CDevice �ʱ�ȭ ����", MB_OK);
		return E_FAIL;
	}

	// FMOD �ʱ�ȭ
	FMOD::System_Create(&m_FMODSystem);
	assert(m_FMODSystem);

	m_FMODSystem->init(32, FMOD_DEFAULT, nullptr);           // ä�� 32�� ����


	// Manager �ʱ�ȭ
	CPathMgr::GetInst()->Init();
	CKeyMgr::GetInst()->Init();
	CTimeMgr::GetInst()->Init();
	CAssetMgr::GetInst()->Init();
	CRenderMgr::GetInst()->Init();

	CLevelMgr::GetInst()->Init();
	CFontMgr::GetInst()->Init();
	CLogMgr::GetInst()->Init();

	// Prefab Function ��� 
	CPrefab::g_ObjectSaveFunc = _SaveFunc;
	CPrefab::g_ObjectLoadFunc = _LoadFunc;

	return S_OK;
}

void CEngine::Progress()
{
	// FMOD Tick
	m_FMODSystem->update();

	// Manager Tick
	CKeyMgr::GetInst()->Tick();
	CTimeMgr::GetInst()->Tick();
	CAssetMgr::GetInst()->Tick();

	// Progress Tick
	CLevelMgr::GetInst()->Progress();

	// Collision
	CCollisionMgr::GetInst()->Tick();

	// Render
	CRenderMgr::GetInst()->Tick();

	// Task
	CTaskMgr::GetInst()->Tick();
}

void CEngine::ChangeWindowScale(UINT _Width, UINT _Height)
{
	bool bMenu = false;
	if (GetMenu(m_hWnd))
		bMenu = true;

	RECT rt = { 0, 0, _Width, _Height };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, bMenu);
	SetWindowPos(m_hWnd, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, 0);
}


