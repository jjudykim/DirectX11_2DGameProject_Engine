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

void CScriptBoxScript::ActiveScriptBox(int _NPCIdx)
{
	if (_NPCIdx == 1) m_ScriptContextIdx = 2;
	else if (_NPCIdx == 2) m_ScriptContextIdx = 3;
	else if (_NPCIdx == 3) m_ScriptContextIdx = 4;
	else if (_NPCIdx == 4) m_ScriptContextIdx = 5;

	Begin();
}

void CScriptBoxScript::Begin()
{
	m_IsScriptActive = true;
	m_IsAbleCloseScript = false;

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
	m_MainCamera->RigidBody()->SetVelocity(Vec3(0.f, 0.f, 0.f));
	m_MainCamera->RigidBody()->SetVelocityByGravity(Vec3(0.f, 0.f, 0.f));

	Vec3 CameraFocus = m_MainCamera->Transform()->GetRelativePos();

	m_NPCPos = Vec3(CameraFocus.x - 440.f, CameraFocus.y - 190.f, 0.f);

	m_BtnE->Transform()->SetRelativeScale(Vec3(50.f, 50.f, 0.f));
	m_BtnE->Transform()->SetRelativePos(CameraFocus.x + 410.f, CameraFocus.y - 325.f, 0.f);

	m_ScriptBox->Transform()->SetRelativeScale(Vec3(850.f, 500.f, 0.f));
	m_ScriptBox->Transform()->SetRelativePos(CameraFocus.x + 80.f, CameraFocus.y - 240.f, 0.f);

	m_BGPostProcess->Transform()->SetRelativePos(CameraFocus);

	CTimeMgr::GetInst()->AddTimer(1.0f, [this]() { m_IsAbleCloseScript = true; }, false);
}

void CScriptBoxScript::Tick()
{
	if (m_IsScriptActive)
	{
		Vec3 CameraFocus = m_MainCamera->Transform()->GetRelativePos();

		if (m_ScriptContextIdx == 1)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Marshmallow");
			m_NPC->Transform()->SetRelativePos(m_NPCPos);
			m_NPC->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
			wstring scriptWstr = L"마쉬멜로우";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"안녕, 드디어 할로우타운에 와줬구나!";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"마을 끝에서 기다리고 있을테니, 준비되면 찾아와줘..";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
		}
		else if (m_ScriptContextIdx == 2)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Marshmallow");
			m_NPC->Transform()->SetRelativePos(m_NPCPos);
			m_NPC->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
			wstring scriptWstr = L"마쉬멜로우";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"오! 이제 모든 준비가 된거야?";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"바로 DarkChat으로 이동하게 될거야!";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
			scriptWstr = L"펌킨슈타인 박사의 광기를 부디 막아줘.. 할로우타운을 위해!";
			wcscpy_s(m_ThirdLineScript, scriptWstr.c_str());
		}
		else if (m_ScriptContextIdx == 3)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Frank");
			m_NPC->Transform()->SetRelativePos(m_NPCPos);
			m_NPC->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
			wstring scriptWstr = L"프랭크";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"음, 무기를 업그레이드 할 수 있는 걸 알고 왔나?";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"하지만 아직은 이른 것 같군..";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
			scriptWstr = L"지금 무기라도 열심히 다뤄보라구.";
			wcscpy_s(m_ThirdLineScript, scriptWstr.c_str());
		}
		else if (m_ScriptContextIdx == 4)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Gates");
			m_NPC->Transform()->SetRelativePos(m_NPCPos);
			m_NPC->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
			wstring scriptWstr = L"게이츠";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"오, 잘 찾아왔구려?";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"공격 방법을 알려줄테니 잘 연습해보셔!";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
			scriptWstr = L"더 강한 데미지를 낼 수 있도록 연구할 수 있을 것이여!";
			wcscpy_s(m_ThirdLineScript, scriptWstr.c_str());
		}
		else if (m_ScriptContextIdx == 5)
		{
			m_NPC = CLevelMgr::GetInst()->FindObjectByName(L"Avatar_Jobs");
			m_NPC->Transform()->SetRelativePos(m_NPCPos);
			m_NPC->Transform()->SetRelativeScale(Vec3(400.f, 400.f, 0.f));
			wstring scriptWstr = L"잡스";
			wcscpy_s(m_NPCName, scriptWstr.c_str());

			scriptWstr = L"네가 그 Scary구나?";
			wcscpy_s(m_FirstLineScript, scriptWstr.c_str());
			scriptWstr = L"마쉬멜로우는 오른쪽에 가면 있을거야. ";
			wcscpy_s(m_SecondLineScript, scriptWstr.c_str());
			scriptWstr = L"음, 뭐. 기대는 하지 않지만 잘 부탁할게.";
			wcscpy_s(m_ThirdLineScript, scriptWstr.c_str());
		}

		CLogMgr::GetInst()->AddUI({ m_NPCName, 420, 520, 30.f, Vec4(255, 255, 255, 255) });

		CLogMgr::GetInst()->AddUI({ m_FirstLineScript, 570, 570, 20.f, Vec4(255, 255, 255, 255) });
		CLogMgr::GetInst()->AddUI({ m_SecondLineScript, 570, 620, 20.f, Vec4(255, 255, 255, 255) });
		CLogMgr::GetInst()->AddUI({ m_ThirdLineScript, 570, 670, 20.f, Vec4(255, 255, 255, 255) });

		if (m_IsAbleCloseScript == true && KEY_TAP(KEY::E))
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