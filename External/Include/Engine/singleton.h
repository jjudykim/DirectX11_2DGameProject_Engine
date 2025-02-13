#pragma once

template<typename T>
class CSingleton
{
private:
	static T* g_Inst;

	// 함수 포인터 타입 EXIT 정의
	typedef void (*EXIT)(void);

public:
	static T* GetInst()
	{
		if (g_Inst == nullptr)
		{
			g_Inst = new T;
		}

		return g_Inst;
	}

	static void Destroy()
	{
		if (g_Inst)
		{
			delete g_Inst;
			g_Inst = nullptr;
		}
	}

public:
	CSingleton()
	{
		atexit((EXIT)&CSingleton<T>::Destroy);
	}

	CSingleton(const CSingleton& _Other) = delete;
	virtual ~CSingleton() {}
};

template<typename T>
T* CSingleton<T>::g_Inst = nullptr;