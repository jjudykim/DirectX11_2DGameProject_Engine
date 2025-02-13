#include "pch.h"
#include "CFSM.h"

#include "CState.h"

CFSM::CFSM()
	: CComponent(COMPONENT_TYPE::FSM)
	, m_CurState(nullptr)
	, m_PrevState(nullptr)
{
}

CFSM::~CFSM()
{
	Delete_Map(m_mapState);
}

void CFSM::FinalTick()
{
	if (m_CurState == nullptr)
		return;

	m_CurState->FinalTick();
}

void CFSM::SetState()
{
	for (const auto& pair : m_mapState)
	{
		pair.second->Set();
	}
}

void CFSM::AddState(const wstring& _Key, CState* _State)
{
	if (m_mapState.find(_Key) == m_mapState.end())
	{
		if (_State != nullptr)
		{
			_State->m_Owner = this;
			_State->SetName(_Key);
			m_vecStateWstr.push_back(_State->GetName());
		}
		m_mapState.insert(make_pair(_Key, _State));
	}
	else
	{
		_State->m_Owner = this;
		m_mapState.find(_Key)->second = _State;
	}
}

CState* CFSM::FindState(const wstring& _Key)
{
	map<wstring, CState*>::iterator iter = m_mapState.find(_Key);

	if (iter == m_mapState.end())
	{
		return nullptr;
	}

	return iter->second;
}

void CFSM::ChangeState(const wstring& _NextStateKey)
{
	if (m_CurState && (_NextStateKey == m_CurState->GetName()))
		return;

	m_PrevState = m_CurState;

	if (m_CurState != nullptr)
		m_CurState->Exit();

	m_CurState = FindState(_NextStateKey);
	assert(m_CurState);

	m_CurState->Enter();
}

void CFSM::SetBlackboardData(const wstring& _DataKey, DATA_TYPE _Type, void* _pData)
{
	if (m_mapData.count(_DataKey) == 0)
	{
		BlackboardData data = { _Type, _pData };
		m_mapData.insert(make_pair(_DataKey, data));
	}
	else
	{
		BlackboardData data = { _Type, _pData };
		m_mapData[_DataKey] = data;
	}
}

void CFSM::SaveToFile(FILE* _File)
{
	
}

void CFSM::LoadFromFile(FILE* _File)
{
	
}

