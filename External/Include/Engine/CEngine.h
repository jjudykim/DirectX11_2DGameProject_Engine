#pragma once

typedef void(*OBJECT_SAVE)(FILE*, class CGameObject*);
typedef CGameObject* (*OBJECT_LOAD)(FILE*);

class CEngine
	: public CSingleton<CEngine>
{
private:
	HWND	         m_hWnd;
	POINT	         m_ptResolution;
	FMOD::System*    m_FMODSystem;
	
private:
	friend class CSingleton<CEngine>;
	CEngine();
	~CEngine();

public:
	HWND GetMainWnd() { return m_hWnd; }

public:
	int Init(HWND _wnd, POINT _ptResolution, OBJECT_SAVE _SaveFunc, OBJECT_LOAD _LoadFunc);
	void Progress();
	void ChangeWindowScale(UINT _Width, UINT _Height);
	FMOD::System* GetFMODSystem() { return m_FMODSystem; }

};

