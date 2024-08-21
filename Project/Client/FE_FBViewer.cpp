#include "pch.h"
#include "FE_FBViewer.h"

FE_FBViewer::FE_FBViewer()
	: m_ViewerSize(ImVec2(300.f, 300.f))
	, m_StartUV(0.0f, 0.0f)
	, m_EndUV(1.0f, 1.0f)
	, m_FrameColor(0.7f, 0.7f, 0.7f, 1.0f)
	, m_ShowFrame(true)
	, m_ShowGrid(true)
{
}

FE_FBViewer::~FE_FBViewer()
{
}

void FE_FBViewer::Init()
{
	m_CurSprite = nullptr;
	m_AtlasTex = nullptr;

	m_ImageRectMin = ImVec2(0.f, 0.f);
	m_ViewerSize = ImVec2(300.f, 300.f);

	m_StartUV = ImVec2(0.f, 0.f);
	m_SliceUV = ImVec2(0.f, 0.f);
	m_OffsetUV = ImVec2(0.f, 0.f);
	m_BackgroundUV = ImVec2(0.f, 0.f);
	m_EndUV = ImVec2(0.f, 0.f);

	m_ShowFrame = true;
	m_ShowGrid = true;
}

void FE_FBViewer::Update()
{
	SetImage();
	

	if (m_ShowFrame)
		m_FrameColor = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	else
		m_FrameColor = ImVec4(1.0f, 1.0f, 1.0f, 0.0f);

	if (m_ShowGrid)
		DrawGrid();

	SelectShowOptions();
}

void FE_FBViewer::SetImage()
{
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - (m_ViewerSize.x * 0.5f));
	ImGui::SetCursorPosY((ImGui::GetWindowSize().y * 0.5f) - (m_ViewerSize.y * 0.75f));
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);

	// Viewer Frame
	if (m_CurSprite != nullptr)
	{
		m_AtlasTex = m_CurSprite->GetAtlasTexture();
		m_OffsetUV = ImVec2(m_CurSprite->GetOffsetUV().x, m_CurSprite->GetOffsetUV().y);
		m_StartUV = ImVec2(m_CurSprite->GetLeftTopUV().x - m_OffsetUV.x, m_CurSprite->GetLeftTopUV().y - m_OffsetUV.y);
		m_SliceUV = ImVec2(m_CurSprite->GetSliceUV().x, m_CurSprite->GetSliceUV().y);
		m_BackgroundUV = ImVec2(m_CurSprite->GetBackgroundUV().x, m_CurSprite->GetBackgroundUV().y);

		ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasTex->Width(), m_CurSprite->GetBackgroundUV().y * m_AtlasTex->Height());

		if (m_ViewerSize.x != crop.x || m_ViewerSize.y != crop.y)
		{
			float ratio = crop.x / m_ViewerSize.x;
			ratio = 1 / ratio;

			crop.x *= ratio;
			crop.y *= ratio;
		}

		m_StartUV = ImVec2((m_StartUV.x + (m_SliceUV.x / 2.f)) - (m_BackgroundUV.x / 2.f)
						 , (m_StartUV.y + (m_SliceUV.y / 2.f)) - (m_BackgroundUV.y / 2.f));
		m_EndUV = ImVec2(m_StartUV.x + m_BackgroundUV.x, m_StartUV.y + m_BackgroundUV.y);

		ImGui::Image(m_AtlasTex->GetSRV().Get(), crop, m_StartUV, m_EndUV, tint_col, m_FrameColor);
	}
	else
	{
		m_StartUV = ImVec2(0.0f, 0.0f);
		m_EndUV = ImVec2(1.0f, 1.0f);
		ImGui::Image(ImTextureID(0), m_ViewerSize, m_StartUV, m_EndUV, tint_col, m_FrameColor);
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

void FE_FBViewer::SelectShowOptions()
{
	ImGui::SetCursorPosY((ImGui::GetWindowSize().y - 100.f));
	ImGui::Text("Show Frame");
	ImGui::SameLine(100.f);
	ImGui::Checkbox("##ShowFrame", &m_ShowFrame);

	ImGui::Text("Show Grid");
	ImGui::SameLine(100.f);
	ImGui::Checkbox("##ShowGrid", &m_ShowGrid);
}