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
	m_vecDataWstr.clear();
	m_vecStateWstr.clear();
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
			_State->m_Owner = this;
		
		m_mapState.insert(make_pair(_Key, _State));
		m_vecStateWstr.push_back(_Key);
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
		m_vecDataWstr.push_back(_DataKey);
	}
	else
	{
		BlackboardData data = { _Type, _pData };
		m_mapData[_DataKey] = data;
	}
}

void CFSM::SaveToFile(FILE* _File)
{
	// Blackboard Data Count / Key
	size_t BlackboardDataCount = m_mapData.size();
	fwrite(&BlackboardDataCount, sizeof(size_t), 1, _File);
	
	for (const auto& pair : m_mapData)
	{
		SaveWString(pair.first, _File);

		DATA_TYPE type = pair.second.Type;
		fwrite(&type, sizeof(DATA_TYPE), 1, _File);
		
		switch (pair.second.Type)
		{
			case DATA_TYPE::INT:
			{
				int data = *((int*)pair.second.pData);
				fwrite(&data, sizeof(int), 1, _File);
				break;
			}
			case DATA_TYPE::FLOAT:
			{
				float data = *((float*)pair.second.pData);
				fwrite(&data, sizeof(float), 1, _File);
				break;
			}
			case DATA_TYPE::VEC3:
			{
				Vec3 data = *((Vec3*)pair.second.pData);
				fwrite(&data, sizeof(Vec3), 1, _File);
				break;
			}
			case DATA_TYPE::WSTRING:
			{
				SaveWString(*((wstring*)pair.second.pData), _File);
				break;
			}
			case DATA_TYPE::UNITVEC_TYPE:
			{
				UINT data = *((UINT*)pair.second.pData);
				fwrite(&data, sizeof(UINT), 1, _File);
				break;
			}
		}
	}

	// Use State Data
	size_t StatesCount = m_mapState.size();
	fwrite(&StatesCount, sizeof(size_t), 1, _File);

	for (const auto& pair : m_mapState)
	{
		SaveWString(pair.first, _File);
	}
}

void CFSM::LoadFromFile(FILE* _File)
{
	// Blackboard Data Count / Key
	size_t BlackboardDataCount = 0;
	fread(&BlackboardDataCount, sizeof(size_t), 1, _File);
	
	for (size_t i = 0; i < BlackboardDataCount; ++i)
	{
		wstring key;
		LoadWString(key, _File);
	
		DATA_TYPE type;
		fread(&type, sizeof(DATA_TYPE), 1, _File);
		
		void* pData = nullptr;
	
		switch (type)
		{
			case DATA_TYPE::INT:
			{
				int data = 0;
				fread(&data, sizeof(int), 1, _File);
				pData = &data;
				break;
			}
			case DATA_TYPE::FLOAT:
			{
				float data = 0.f;
				fread(&data, sizeof(float), 1, _File);
				pData = &data;
				break;
			}
			case DATA_TYPE::VEC3:
			{
				Vec3 data = Vec3(0.f, 0.f, 0.f);
				fread(&data, sizeof(Vec3), 1, _File);
				pData = &data;
				break;
			}
			case DATA_TYPE::WSTRING:
			{
				wstring data;
				LoadWString(data, _File);
				pData = &data;
				break;
			}
			case DATA_TYPE::UNITVEC_TYPE:
			{
				UINT data = 0;
				fread(&data, sizeof(UINT), 1, _File);
				pData = &data;
				break;
			}
		}
	
		SetBlackboardData(key, (DATA_TYPE)type, pData);
	}
	
	size_t StatesCount = 0;
	fread(&StatesCount, sizeof(size_t), 1, _File);
	
	wstring state;
	for (size_t i = 0; i < StatesCount; ++i)
	{
		LoadWString(state, _File);
		AddState(state, nullptr);
	}
}

