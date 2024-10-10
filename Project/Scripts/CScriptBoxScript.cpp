#include "pch.h"
#include "CScriptBoxScript.h"

#include "Engine\CFontMgr.h"
#include "Engine\CLevelMgr.h"
#include "Engine\CLogMgr.h"

CScriptBoxScript::CScriptBoxScript()
	: CScript(SCRIPT_TYPE::SCRIPTBOXSCRIPT)
{
	AddScriptParam(SCRIPT_PARAM::INT, "Script Context", &m_ScriptContextIdx);
}

CScriptBoxScript::~CScriptBoxScript()
{
}

void CScriptBoxScript::Begin()
{
	m_IsScriptActive = true;

	wstring emptyWstr = L"";
	wcscpy_s(m_FirstLineScript, emptyWstr.c_str());
	wcscpy_s(m_SecondLineScript, emptyWstr.c_str());
	wcscpy_s(m_ThirdLineScript, emptyWstr.c_str());

	wstring blurKey = L"prefab\\BokehBlurPPPref.pref";
	Ptr<CPrefab> bokehBlurPref = CAssetMgr::GetInst()->FindAsset<CPrefab>(blurKey);
	if (bokehBlurPref == nullptr)
		bokehBlurPref = CAssetMgr::GetInst()->Load<CPrefab>(blurKey, blurKey);

	m_BGPostProcess = bokehBlurPref->Instantiate();
	CTaskMgr::GetInst()->AddTask({ TASK_TYPE::CREATE_OBJECT, (DWORD_PTR)26, (DWORD_PTR)m_BGPostProcess });

	m_MainCamera = CLevelMgr::GetInst()->FindObjectByName(L"MainCamera");
	m_BtnE = CLevelMgr::GetInst()->FindObjectByName(L"UI_BtnE");
	m_ScriptBox = CLevelMgr::GetInst()->FindObjectByName(L"UI_ScriptBox");
	m_NPC = nullptr;


	// Setting
	m_MainCamera->FSM()->ChangeState(L"NoMove");

	Vec3 CameraFocus = m_MainCamera->Transform()->GetRelativePos();

	m_BtnE->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 0.f));
	m_BtnE->Transform()->SetRelativePos(CameraFocus.x + 410.f, CameraFocus.y - 325.f, 0.f);

	m_ScriptBox->Transform()->SetRelativeScale(Vec3(850.f, 500.f, 0.f));
	m_ScriptBox->Transform()->SetRelativePos(CameraFocus.x + 80.f, CameraFocus.y - 240.f, 0.f);

}

void CScriptBoxScript::Tick()
{
	if (m_IsScriptActive)
	{
		Vec3 CameraFocus = m_MainCamera->Transform()->GetRelativePos();

		if (m_ScriptContextIdx == 1)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Marshmallow");
			wstring scriptWstr = L"마쉬멜로우";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"안녕, 드디어 할로우타운에 와줬구나!";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"마을 끝에서 기다리고 있을테니, 준비되면 찾아와줘..";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
		}

		CLogMgr::GetInst()->AddUI({ m_NPCName, CameraFocus.x + 200, CameraFocus.y + 220, 30.f, Vec4(255, 255, 255, 255) });

		CLogMgr::GetInst()->AddUI({ m_FirstLineScript, CameraFocus.x + 350, CameraFocus.y + 300, 20.f, Vec4(255, 255, 255, 255) });
		CLogMgr::GetInst()->AddUI({ m_SecondLineScript, CameraFocus.x + 350, CameraFocus.y + 350, 20.f, Vec4(255, 255, 255, 255) });
		CLogMgr::GetInst()->AddUI({ m_ThirdLineScript, CameraFocus.x + 350, CameraFocus.y + 400, 20.f, Vec4(255, 255, 255, 255) });

		if (KEY_TAP(KEY::E))
		{
			CTaskMgr::GetInst()->AddTask({ TASK_TYPE::DELETE_OBJECT, (DWORD_PTR)m_BGPostProcess });

			m_BtnE->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
			m_ScriptBox->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
			m_NPC->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));

			m_MainCamera->FSM()->ChangeState(L"ChasingPlayer");

			m_IsScriptActive = false;
		}
	}
}

void CScriptBoxScript::SaveToFile(FILE* _File)
{
	fwrite(&m_ScriptContextIdx, sizeof(int), 1, _File);
}

void CScriptBoxScript::LoadFromFile(FILE* _File)
{
	fread(&m_ScriptContextIdx, sizeof(int), 1, _File);
}