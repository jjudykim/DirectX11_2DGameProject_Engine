#include "pch.h"
#include "FlipBookEditor.h"

#include "CEditorMgr.h"
#include "FE_FBViewer.h"
#include "FE_FBDetail.h"
#include "FE_SpriteList.h"

FlipBookEditor::FlipBookEditor()
	: m_Viewer(nullptr)
	, m_Detail(nullptr)
	, m_SpriteList(nullptr)
{
	UseMenuBar(true);
}

FlipBookEditor::~FlipBookEditor()
{
}

void FlipBookEditor::Init()
{
	m_Viewer = (FE_FBViewer*)CEditorMgr::GetInst()->FindEditorUI("FE_FBViewer");
	m_Detail = (FE_FBDetail*)CEditorMgr::GetInst()->FindEditorUI("FE_FBDetail");
	m_SpriteList = (FE_SpriteList*)CEditorMgr::GetInst()->FindEditorUI("FE_SpriteList");

	m_Viewer->SetMove(true);
	m_Detail->SetMove(true);
	m_SpriteList->SetMove(true);

	m_Viewer->m_Owner = this;
	m_Detail->m_Owner = this;
	m_SpriteList->m_Owner = this;
}

void FlipBookEditor::Update()
{
	if (ImGui::BeginMenuBar())
	{
		if (ImGui::BeginMenu("Window"))
		{
			bool Viewer = m_Viewer->IsActive();
			bool Detail = m_Detail->IsActive();
			bool SpriteList = m_SpriteList->IsActive();

			if (ImGui::MenuItem("FlipBook Viewer", nullptr, &Viewer))
			{
				m_Viewer->SetActive(Viewer);
			}

			if (ImGui::MenuItem("FlipBook Detail", nullptr, &Detail))
			{
				m_Detail->SetActive(Detail);
			}

			if (ImGui::MenuItem("Sprite List", nullptr, &SpriteList))
			{
				m_SpriteList->SetActive(SpriteList);
			}

			ImGui::EndMenu();
		}

		ImGui::EndMenuBar();
	}
}

void FlipBookEditor::Activate()
{
	m_Viewer->SetActive(true);
	m_Detail->SetActive(true);
	m_SpriteList->SetActive(true);
}

void FlipBookEditor::Deactivate()
{
	m_Viewer->SetActive(false);
	m_Detail->SetActive(false);
	m_SpriteList->SetActive(false);
}

