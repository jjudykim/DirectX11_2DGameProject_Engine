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
		m_FSM = (CFSM*)GetTargetObject()->GetComponent(COMPONENT_TYPE::FSM);

	vector<wstring> vecStateWstr = m_FSM->GetVecStateWstr();
	vector<wstring> vecDataWStr = m_FSM->GetVecDataWstr();

	ImGui::SeparatorText("State List");
	if (ImGui::BeginListBox("##StateList", ImVec2(-FLT_MIN, 100.f)))
	{
		for (size_t i = 0; i < vecStateWstr.size(); i++)
		{
			const bool is_selected = 0;
			string label = string(vecStateWstr[i].begin(), vecStateWstr[i].end());
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
		for (size_t i = 0; i < vecDataWStr.size(); i++)
		{
			const bool is_selected = 0;
			string label = string(vecDataWStr[i].begin(), vecDataWStr[i].end());
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


