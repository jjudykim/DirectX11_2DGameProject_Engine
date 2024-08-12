#include "pch.h"
#include "CGameObjectEx.h"

void CGameObjectEx::FinalTick()
{
	for (UINT i = 0; i < (UINT)COMPONENT_TYPE::END; ++i)
	{
		if (GetComponent((COMPONENT_TYPE)i) != nullptr)
			GetComponent((COMPONENT_TYPE)i)->FinalTick();
	}

	// Child Object
	vector<CGameObject*>::const_iterator iter = GetChildren().begin();
	for (; iter != GetChildren().end(); ++iter)
	{
		(*iter)->FinalTick();
	}
}
