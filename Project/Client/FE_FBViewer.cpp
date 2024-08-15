#include "pch.h"
#include "FE_FBViewer.h"

FE_FBViewer::FE_FBViewer()
	: m_ViewerSize(ImVec2(450.f, 450.f))
{
}

FE_FBViewer::~FE_FBViewer()
{
}

void FE_FBViewer::Init()
{
}

void FE_FBViewer::Update()
{
	DrawGrid();
}

void FE_FBViewer::DrawGrid()
{
	m_ImageRectMin = ImGui::GetItemRectMin();

	ImGui::GetWindowDrawList()->AddLine(ImVec2(m_ImageRectMin.x + (m_ViewerSize.x / 2.f), m_ImageRectMin.y)
									  , ImVec2(m_ImageRectMin.x + (m_ViewerSize.x / 2.f), m_ImageRectMin.y + m_ViewerSize.y)
		                              , ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f)), 1.f);

	ImGui::GetWindowDrawList()->AddLine(ImVec2(m_ImageRectMin.x, m_ImageRectMin.y + (m_ViewerSize.y / 2.f))
									  , ImVec2(m_ImageRectMin.x + m_ViewerSize.x, m_ImageRectMin.y + (m_ViewerSize.y / 2.f))
									  , ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f)), 1.f);
}
