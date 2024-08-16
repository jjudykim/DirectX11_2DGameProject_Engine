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
	if (m_CurSprite == nullptr)
	{

	}
	else
	{
	}

	SetImage();
	DrawGrid();
}

void FE_FBViewer::SetImage()
{
	// Viewer Frame
	if (m_CurSprite != nullptr)
	{
		ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasTex->Width(), m_CurSprite->GetBackgroundUV().y * m_AtlasTex->Height());

		if (m_ViewerSize.x != crop.x || m_ViewerSize.y != crop.y)
		{
			float ratio = crop.x / m_ViewerSize.x;
			ratio = 1 / ratio;

			crop.x *= ratio;
			crop.y *= ratio;

			ImVec2 StartUV = ImVec2(m_CurSprite->GetLeftTopUV().x, m_CurSprite->GetLeftTopUV().y);
			ImVec2 EndUV = ImVec2(m_CurSprite->GetLeftTopUV().x + m_CurSprite->GetSliceUV().x
				, m_CurSprite->GetLeftTopUV().y + m_CurSprite->GetSliceUV().y);

			//ImGui::Image(m_AtlasTex->GetSRV().Get(), crop, StartUV, EndUV, tint_col, border_col);
		}
	}
	

	
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
