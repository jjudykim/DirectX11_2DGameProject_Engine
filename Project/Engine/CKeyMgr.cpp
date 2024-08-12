#include "pch.h"
#include "CKeyMgr.h"

#include "CEngine.h"

UINT g_RealKey[(UINT)KEY::KEY_END] =
{
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
	'Q', 'W', 'E', 'R', 'T', 'Y', 'U', 'I', 'O', 'P',
	'A', 'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L',
	'Z', 'X', 'C', 'V', 'B', 'N', 'M',

	VK_LEFT, VK_RIGHT, VK_UP, VK_DOWN,

	VK_RETURN,
	VK_SPACE,
	VK_MENU,
	VK_CONTROL,
	VK_LSHIFT,
	VK_ESCAPE,

	VK_LBUTTON,
	VK_RBUTTON,
};

CKeyMgr::CKeyMgr()
{
}

CKeyMgr::~CKeyMgr()
{
}

void CKeyMgr::Init()
{
	for (int i = 0; i < (int)KEY::KEY_END; ++i)
	{
		tKeyInfo info = {};
		info.eKey = (KEY)i;
		info.eKeyState = KEY_STATE::NONE;
		info.bPressed = false;

		m_vecKeyInfo.push_back(info);
	}
}

void CKeyMgr::Tick()
{
	if (CEngine::GetInst()->GetMainWnd() == GetFocus())
	{
		for (UINT i = 0; i < (UINT)m_vecKeyInfo.size(); ++i)
		{
			if (0x8001 & GetAsyncKeyState(g_RealKey[(UINT)m_vecKeyInfo[i].eKey]))
			{
				if (m_vecKeyInfo[i].bPressed)
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::PRESSED;
				}
				else
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::TAP;
				}

				m_vecKeyInfo[i].bPressed = true;
			}
			else
			{
				if (m_vecKeyInfo[i].bPressed)
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::RELEASED;
				}
				else
				{
					m_vecKeyInfo[i].eKeyState = KEY_STATE::NONE;
				}

				m_vecKeyInfo[i].bPressed = false;
			}
		}

		// 마우스 좌표 계산
		if (m_MouseCapture)
		{
			POINT ptMousePos = {};
			GetCursorPos(&ptMousePos);
			ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
			m_MousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);

			m_DragDir = m_MousePos - m_CapturePos;

			POINT ptCapturePos = { (int)m_CapturePos.x, (int)m_CapturePos.y };
			ClientToScreen(CEngine::GetInst()->GetMainWnd(), &ptCapturePos);
			SetCursorPos(ptCapturePos.x, ptCapturePos.y);
		}
		else
		{
			m_PrevMousePos = m_MousePos;
			POINT ptMousePos = {};
			GetCursorPos(&ptMousePos);
			ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
			m_MousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);
			m_DragDir = m_MousePos - m_PrevMousePos;
		}
		/*m_PrevMousePos = m_MousePos;

		POINT ptMousePos = { };
		GetCursorPos(&ptMousePos);
		ScreenToClient(CEngine::GetInst()->GetMainWnd(), &ptMousePos);
		m_MousePos = Vec2((float)ptMousePos.x, (float)ptMousePos.y);
		m_DragDir = m_MousePos - m_PrevMousePos;*/
	}

	else
	{
		for (UINT i = 0; i < (UINT)m_vecKeyInfo.size(); ++i)
		{
			if (m_vecKeyInfo[i].eKeyState == KEY_STATE::TAP
				|| m_vecKeyInfo[i].eKeyState == KEY_STATE::PRESSED)
			{
				m_vecKeyInfo[i].eKeyState = KEY_STATE::RELEASED;
			}
			else
			{
				m_vecKeyInfo[i].eKeyState = KEY_STATE::NONE;
			}

			m_vecKeyInfo[i].bPressed = false;
		}
	}
}
