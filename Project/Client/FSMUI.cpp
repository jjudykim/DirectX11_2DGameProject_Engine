#include "pch.h"
#include "FSMUI.h"

#include "Engine/CState.h"

FSMUI::FSMUI()
	: ComponentUI(COMPONENT_TYPE::FSM)
{
}

FSMUI::~FSMUI()
{
}

void FSMUI::Init()
{
}

void FSMUI::Update()
{
	Title();

	m_FSM = (CFSM*)GetTargetObject()->GetComponent(COMPONENT_TYPE::FSM);
	
	if (GetTargetObject()->IsDead())
		return;

	string CurState = "";
	if (m_FSM->GetCurState() != nullptr)
	{
		 CurState = string(m_FSM->GetCurState()->GetName().begin(), m_FSM->GetCurState()->GetName().end());
	}

	ImGui::Text("Cur State");
	ImGui::SameLine(120.f);
	ImGui::InputText("##CurStateName", (char*)CurState.c_str(), ImGuiInputTextFlags_ReadOnly);

	m_vecStateWstr = m_FSM->GetVecStateWstr();
	
	ImGui::SeparatorText("State List");
	if (ImGui::BeginListBox("##StateList", ImVec2(-FLT_MIN, 100.f)))
	{
		for (size_t i = 0; i < m_vecStateWstr.size(); i++)
		{
			const bool is_selected = 0;

			string label = "##" + std::to_string(i);

			if (m_vecStateWstr[i].length() > 0)
				label = string(m_vecStateWstr[i].begin(), m_vecStateWstr[i].end());
			
			if (ImGui::Selectable(label.c_str(), is_selected))
			{
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
			}
		}
		ImGui::EndListBox();
	}
}


