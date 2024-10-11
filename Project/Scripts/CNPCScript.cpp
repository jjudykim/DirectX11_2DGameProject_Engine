#include "pch.h"
#include "CNPCScript.h"

#include "Engine\CLevelMgr.h"
#include "Engine\CLevel.h"
#include "Engine\CScript.h"

#include "CScriptBoxScript.h"

static int CurTalkingNPC = 0;
static bool TalkingWithNPC = false;

CNPCScript::CNPCScript()
	: CScript(SCRIPT_TYPE::NPCSCRIPT)
{
	
	AddScriptParam(SCRIPT_PARAM::INT, "NPCType", &m_NPCType);
	AddScriptParam(SCRIPT_PARAM::VEC3, "TalkBtn Vec3", &m_vTalkBtn);
}

CNPCScript::~CNPCScript()
{
}

void CNPCScript::Begin()
{
	m_TalkBtn = CLevelMgr::GetInst()->FindObjectByName(L"NPC_BtnE");
	m_ScriptBox = CLevelMgr::GetInst()->FindObjectByName(L"UI_ScriptBox");
	m_TalkBtn->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	m_TalkBtn->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
}

void CNPCScript::Tick()
{
	if (m_IsAbleToTalk == true)
	{
		m_TalkBtn->Transform()->SetRelativePos(m_vTalkBtn);
		m_TalkBtn->Transform()->SetRelativeScale(Vec3(60.f, 60.f, 0.f));

		CScriptBoxScript* Script = (CScriptBoxScript*)m_ScriptBox->GetScriptByIndex(0);

		if (CurTalkingNPC == m_NPCType && KEY_TAP(KEY::E))
		{
			if (!TalkingWithNPC)
			{
				TalkingWithNPC = true;
				Script->ActiveScriptBox(CurTalkingNPC);
			}
		}

		if (TalkingWithNPC)
		{
			if (!Script->IsScriptActive())
				TalkingWithNPC = false;
		}
	}
}

void CNPCScript::SaveToFile(FILE* _File)
{
	fwrite(&m_NPCType, sizeof(int), 1, _File);
	fwrite(&m_vTalkBtn, sizeof(Vec3), 1, _File);
}

void CNPCScript::LoadFromFile(FILE* _File)
{
	fread(&m_NPCType, sizeof(int), 1, _File);
	fread(&m_vTalkBtn, sizeof(Vec3), 1, _File);
}

void CNPCScript::BeginOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	m_IsAbleToTalk = true;
	CurTalkingNPC = m_NPCType;
}

void CNPCScript::Overlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
}

void CNPCScript::EndOverlap(CCollider2D* _OwnCollider, CGameObject* _OtherObject, CCollider2D* _OtherCollider)
{
	//if (CurTalkingNPC != 0)
	//{
	//	if (CurTalkingNPC == m_NPCType)
	//	{
	//		m_IsAbleToTalk = false;
	//		m_TalkBtn->Transform()->SetRelativePos(Vec3(0.f, 0.f, 0.f));
	//		m_TalkBtn->Transform()->SetRelativeScale(Vec3(0.f, 0.f, 0.f));
	//	}
	//}
}

