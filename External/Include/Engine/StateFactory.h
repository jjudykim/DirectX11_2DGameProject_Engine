#pragma once
#include <functional>

class StateFactory
	: public CSingleton<StateFactory>
{
	SINGLE(StateFactory)

public:
	using CreateStateFn = std::function<CState*()>;

private:
	map<wstring, CreateStateFn> stateRegistry;

public:
	void Init();

	void RegisterState(const std::wstring& name, CreateStateFn createFn) 
	{
		stateRegistry[name] = createFn;
	}

	CState* CreateState(const std::wstring& name) 
	{
		auto it = stateRegistry.find(name);

		if (it != stateRegistry.end())
			return it->second();            // 등록된 생성 함수 호출
		
		return nullptr;                     // 등록되지 않은 상태 이름일 경우
	}
};

