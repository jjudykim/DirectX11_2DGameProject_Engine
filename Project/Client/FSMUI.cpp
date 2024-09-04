#include "pch.h"
#include "FSMUI.h"

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

	if (m_FSM == nullptr)
	{
		m_FSM = (CFSM*)GetTargetObject()->GetComponent(COMPONENT_TYPE::FSM);
	}
	else
	{
		m_vecStateWstr = m_FSM->GetVecStateWstr();
		m_vecDataWStr = m_FSM->GetVecDataWstr();
	}

	ImGui::SeparatorText("State List");
	if (ImGui::BeginListBox("##StateList", ImVec2(-FLT_MIN, 100.f)))
	{
		for (size_t i = 0; i < m_vecStateWstr.size(); i++)
		{
			const bool is_selected = 0;
			string label = string(m_vecStateWstr[i].begin(), m_vecStateWstr[i].end());
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

	ImGui::Text("");

	ImGui::SeparatorText("Blackboard List");
	if (ImGui::BeginListBox("##BlackboardList", ImVec2(-FLT_MIN, 100.f)))
	{
		for (size_t i = 0; i < m_vecDataWStr.size(); i++)
		{
			const bool is_selected = 0;
			string label = string(m_vecDataWStr[i].begin(), m_vecDataWStr[i].end());
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


