#include "pch.h"
#include "CScriptMgr.h"

#include "CArcProjectileScript.h"
#include "CBackgroundMoveScript.h"
#include "CBossScript.h"
#include "CBouncingProjectileScript.h"
#include "CCameraMoveScript.h"
#include "CLoadingIconScript.h"
#include "CMissileScript.h"
#include "CMonsterScript.h"
#include "CNPCScript.h"
#include "CPlayerScript.h"
#include "CScriptBoxScript.h"
#include "CTitleMenuScript.h"

void CScriptMgr::GetScriptInfo(vector<wstring>& _vec)
{
	_vec.push_back(L"CArcProjectileScript");
	_vec.push_back(L"CBackgroundMoveScript");
	_vec.push_back(L"CBossScript");
	_vec.push_back(L"CBouncingProjectileScript");
	_vec.push_back(L"CCameraMoveScript");
	_vec.push_back(L"CLoadingIconScript");
	_vec.push_back(L"CMissileScript");
	_vec.push_back(L"CMonsterScript");
	_vec.push_back(L"CNPCScript");
	_vec.push_back(L"CPlayerScript");
	_vec.push_back(L"CScriptBoxScript");
	_vec.push_back(L"CTitleMenuScript");
}

CScript * CScriptMgr::GetScript(const wstring& _strScriptName)
{
	if (L"CArcProjectileScript" == _strScriptName)
		return new CArcProjectileScript;
	if (L"CBackgroundMoveScript" == _strScriptName)
		return new CBackgroundMoveScript;
	if (L"CBossScript" == _strScriptName)
		return new CBossScript;
	if (L"CBouncingProjectileScript" == _strScriptName)
		return new CBouncingProjectileScript;
	if (L"CCameraMoveScript" == _strScriptName)
		return new CCameraMoveScript;
	if (L"CLoadingIconScript" == _strScriptName)
		return new CLoadingIconScript;
	if (L"CMissileScript" == _strScriptName)
		return new CMissileScript;
	if (L"CMonsterScript" == _strScriptName)
		return new CMonsterScript;
	if (L"CNPCScript" == _strScriptName)
		return new CNPCScript;
	if (L"CPlayerScript" == _strScriptName)
		return new CPlayerScript;
	if (L"CScriptBoxScript" == _strScriptName)
		return new CScriptBoxScript;
	if (L"CTitleMenuScript" == _strScriptName)
		return new CTitleMenuScript;
	return nullptr;
}

CScript * CScriptMgr::GetScript(UINT _iScriptType)
{
	switch (_iScriptType)
	{
	case (UINT)SCRIPT_TYPE::ARCPROJECTILESCRIPT:
		return new CArcProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::BACKGROUNDMOVESCRIPT:
		return new CBackgroundMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::BOSSSCRIPT:
		return new CBossScript;
		break;
	case (UINT)SCRIPT_TYPE::BOUNCINGPROJECTILESCRIPT:
		return new CBouncingProjectileScript;
		break;
	case (UINT)SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return new CCameraMoveScript;
		break;
	case (UINT)SCRIPT_TYPE::LOADINGICONSCRIPT:
		return new CLoadingIconScript;
		break;
	case (UINT)SCRIPT_TYPE::MISSILESCRIPT:
		return new CMissileScript;
		break;
	case (UINT)SCRIPT_TYPE::MONSTERSCRIPT:
		return new CMonsterScript;
		break;
	case (UINT)SCRIPT_TYPE::NPCSCRIPT:
		return new CNPCScript;
		break;
	case (UINT)SCRIPT_TYPE::PLAYERSCRIPT:
		return new CPlayerScript;
		break;
	case (UINT)SCRIPT_TYPE::SCRIPTBOXSCRIPT:
		return new CScriptBoxScript;
		break;
	case (UINT)SCRIPT_TYPE::TITLEMENUSCRIPT:
		return new CTitleMenuScript;
		break;
	}
	return nullptr;
}

const wchar_t * CScriptMgr::GetScriptName(CScript * _pScript)
{
	switch ((SCRIPT_TYPE)_pScript->GetScriptType())
	{
	case SCRIPT_TYPE::ARCPROJECTILESCRIPT:
		return L"CArcProjectileScript";
		break;

	case SCRIPT_TYPE::BACKGROUNDMOVESCRIPT:
		return L"CBackgroundMoveScript";
		break;

	case SCRIPT_TYPE::BOSSSCRIPT:
		return L"CBossScript";
		break;

	case SCRIPT_TYPE::BOUNCINGPROJECTILESCRIPT:
		return L"CBouncingProjectileScript";
		break;

	case SCRIPT_TYPE::CAMERAMOVESCRIPT:
		return L"CCameraMoveScript";
		break;

	case SCRIPT_TYPE::LOADINGICONSCRIPT:
		return L"CLoadingIconScript";
		break;

	case SCRIPT_TYPE::MISSILESCRIPT:
		return L"CMissileScript";
		break;

	case SCRIPT_TYPE::MONSTERSCRIPT:
		return L"CMonsterScript";
		break;

	case SCRIPT_TYPE::NPCSCRIPT:
		return L"CNPCScript";
		break;

	case SCRIPT_TYPE::PLAYERSCRIPT:
		return L"CPlayerScript";
		break;

	case SCRIPT_TYPE::SCRIPTBOXSCRIPT:
		return L"CScriptBoxScript";
		break;

	case SCRIPT_TYPE::TITLEMENUSCRIPT:
		return L"CTitleMenuScript";
		break;

	}
	return nullptr;
}