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
	if (FindState(_Key) == nullptr)
	{
		_State->m_Owner = this;
		m_mapState.insert(make_pair(_Key, _State));
	};
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

void CFSM::LoadFromFile(FILE* _File)
{
}

void CFSM::SaveToFile(FILE* _File)
{
}