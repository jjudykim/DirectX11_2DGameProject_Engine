#include "pch.h"
#include "CLayer.h"

#include "CGameObject.h"

CLayer::CLayer(int _LayerIdx)
	: m_LayerIdx(_LayerIdx)
{
	string strLayerName = ToString(_LayerIdx);
	SetName(wstring(strLayerName.begin(), strLayerName.end()));
}

CLayer::CLayer(const CLayer& _Origin)
	: CEntity(_Origin)
	, m_LayerIdx(_Origin.m_LayerIdx)
{
	for (size_t i = 0; i < _Origin.m_Parents.size(); ++i)
	{
		AddObject(_Origin.m_Parents[i]->Clone(), false);
	}
}

CLayer::~CLayer()
{
	Delete_Vec(m_Parents);
}

void CLayer::Begin()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Begin();
	}
}

void CLayer::Tick()
{
	for (size_t i = 0; i < m_Parents.size(); ++i)
	{
		m_Parents[i]->Tick();
	}
}

void CLayer::FinalTick()
{
	vector<CGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end();)
	{
		(*iter)->FinalTick();

		if ((*iter)->IsDead())
			iter = m_Parents.erase(iter);
		else
			++iter;
	}
}

void CLayer::AddObject(CGameObject* _Object, bool _bChildMove)
{
	// 1. 오브젝트가 다른 Layer의 소속인 경우
	if (_Object->GetLayerIdx() != -1)
	{
		assert(_Object->GetLayerIdx() != m_LayerIdx);

		// 기존에 소속되어있던 레이어에서 빠져나와야 함
		_Object->DisconnectWithLayer();
	}

	// 2. 최상위 오브젝트인 경우
	if (_Object->GetParent() == nullptr)
	{
		m_Parents.push_back(_Object);
	}
	// 자식들까지 이동시킬지?
	static list<CGameObject*> queue;
	queue.clear();
	queue.push_back(_Object);
	_Object->m_LayerIdx = m_LayerIdx;

	while (!queue.empty())
	{
		CGameObject* pObject = queue.front();
		queue.pop_front();

		if (_bChildMove || pObject->m_LayerIdx == -1)
		{
			pObject->m_LayerIdx = m_LayerIdx;
		}

		const vector<CGameObject*>& vecChildren = pObject->GetChildren();
		for (size_t i = 0; i < vecChildren.size(); ++i)
		{
			queue.push_back(vecChildren[i]);
		}
	}
}

void CLayer::DeregisterObject(CGameObject* _Object)
{
	assert(_Object->m_LayerIdx == m_LayerIdx);

	_Object->m_LayerIdx = -1;

	if (_Object->GetParent() == nullptr)
	{
		vector<CGameObject*>::iterator iter = m_Parents.begin();

		for (; iter != m_Parents.end(); ++iter)
		{
			if (_Object == (*iter))
			{
				m_Parents.erase(iter);
				return;
			}
		}
		assert(nullptr);
	}
}

void CLayer::RegisterAsParent(CGameObject* _Object)
{
	assert(_Object->GetLayerIdx() == m_LayerIdx && !_Object->GetParent());

	m_Parents.push_back(_Object);
	return;
}

void CLayer::DeregisterObjectAsParent(CGameObject* _Object)
{
	vector<CGameObject*>::iterator iter = m_Parents.begin();

	for (; iter != m_Parents.end(); ++iter)
	{
		if (_Object == (*iter))
		{
			m_Parents.erase(iter);
			return;
		}
	}
	assert(nullptr);
}

void CLayer::DisconnectWithObject(CGameObject* _Object)
{
	vector<CGameObject*>::iterator iter = m_Parents.begin();
	for (; iter != m_Parents.end(); ++iter)
	{
		if (_Object == (*iter))
		{
			m_Parents.erase(iter);
			return;
		}
	}

	assert(nullptr);
}