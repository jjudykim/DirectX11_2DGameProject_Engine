#include "pch.h"
#include "SE_AtlasView.h"

#include "SE_Detail.h"

#include <Engine/CKeyMgr.h>

SE_AtlasView::SE_AtlasView()
	: m_WidthSize(200)
	, m_WheelScale(1.f)
{
}

SE_AtlasView::~SE_AtlasView()
{
}

void SE_AtlasView::Init()
{
}

void SE_AtlasView::Update()
{
	if (m_AtlasTex == nullptr)
		return;

	WheelCheck();

	// Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	// 실제 Resolution 대비 Output Image의 비율
	m_Ratio = (m_WidthSize * m_WheelScale) / m_AtlasTex->Width();

	ImGui::Image(m_AtlasTex->GetSRV().Get(), ImVec2((m_WidthSize * m_WheelScale)
													, m_AtlasTex->Height() * m_Ratio)
													, uv_min, uv_max, tint_col, border_col);

	// SelectCheck
	SelectCheck();

	// Select Area에 Rect Draw
	DrawSelectRect();
}

void SE_AtlasView::SetAtlasTex(Ptr<CTexture> _Tex)
{
	if (m_AtlasTex == _Tex)
		return;

	m_AtlasTex = _Tex;

	if (m_AtlasTex == nullptr)
		return;

	m_WidthSize = (float)m_AtlasTex->Width();
}

void SE_AtlasView::WheelCheck()
{
	if (0 < ImGui::GetIO().MouseWheel && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
		m_WheelScale += 0.05f;

	if (0 > ImGui::GetIO().MouseWheel && ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl))
		m_WheelScale -= 0.05f;

	if (3.f < m_WheelScale)
		m_WheelScale = 3.f;

	if (m_WheelScale < 0.1f)
		m_WheelScale = 0.1f;
}

void SE_AtlasView::SelectCheck()
{
	// Image 위젯 좌상단 좌표
	m_ImageRectMin = ImGui::GetItemRectMin();
	float ArrImageMin[] = { m_ImageRectMin.x, m_ImageRectMin.y };
	ImGui::InputFloat2("ImageMin", ArrImageMin);

	// Current Mouse Position
	m_MousePos = ImGui::GetMousePos();
	float arrMousePos[] = { m_MousePos.x, m_MousePos.y };
	ImGui::InputFloat2("MousePos", arrMousePos);

	ImVec2 vDiff = ImVec2(m_MousePos.x - m_ImageRectMin.x, m_MousePos.y - m_ImageRectMin.y);
	vDiff = ImVec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

	// Mouse 위치의 Atlas Pixel Position
	float PixelPos[] = { vDiff.x, vDiff.y };
	ImGui::InputFloat2("PixelPos", PixelPos);

	// Left Mouse Click Check
	if (ImGui::IsMouseClicked(ImGuiMouseButton_Left)) //KEY_RELEASED(KEY::LBTN)
	{
		Vec2 vPixelPos = Vec2(ImGui::GetMousePos().x, ImGui::GetMousePos().y);
		ImVec2 vDiff = ImVec2(vPixelPos.x - m_ImageRectMin.x, vPixelPos.y - m_ImageRectMin.y);
		vPixelPos = Vec2(vDiff.x / m_Ratio, vDiff.y / m_Ratio);

		if (0.f <= vPixelPos.x && vPixelPos.x < m_AtlasTex->Width()
			&& 0.f <= vPixelPos.y && vPixelPos.y < m_AtlasTex->Height())
		{
			CalcSpriteSize(vPixelPos);
			GetDetail()->SetSelectedSprite(false);
		}
	}
}

void SE_AtlasView::DrawSelectRect()
{
	ImVec2 MouseLTPos = ImVec2(m_MouseLT.x * m_Ratio + m_ImageRectMin.x, m_MouseLT.y * m_Ratio + m_ImageRectMin.y);
	ImVec2 MouseRBPos = ImVec2(m_MouseRB.x * m_Ratio + m_ImageRectMin.x, m_MouseRB.y * m_Ratio + m_ImageRectMin.y);

	ImGui::GetWindowDrawList()->AddRect(MouseLTPos, MouseRBPos
										, ImGui::GetColorU32(ImVec4(1.f, 1.f, 0.f, 1.f))
										, 0.f, 0.f, 1.f);
}

void SE_AtlasView::CalcSpriteSize(Vec2 _PixelPos)
{
	float left = m_AtlasTex->Width() - 1.f;
	float top = m_AtlasTex->Height() - 1.f;
	float right = 0.f;
	float bot = 0.f;

	m_PixelID.clear();

	list<Vec2> queue;
	queue.push_back(_PixelPos);

	while (!queue.empty())
	{
		Vec2 vPixelPos = queue.front();
		queue.pop_front();

		if (vPixelPos.x < left)
			left = vPixelPos.x;
		if (vPixelPos.x > right)
			right = vPixelPos.x;
		if (vPixelPos.y < top)
			top = vPixelPos.y;
		if (vPixelPos.y > bot)
			bot = vPixelPos.y;

		// 주변 픽셀을 queue에 추가
		Vec2 vUp = vPixelPos + Vec2(0.f, 1.f);      // 위
		Vec2 vDown = vPixelPos + Vec2(0.f, -1.f);   // 아래
		Vec2 vLeft = vPixelPos + Vec2(-1.f, 0.f);   // 좌
		Vec2 vRight = vPixelPos + Vec2(1.f, 0.f);   // 우

		if (IsPixelOk(vUp))
		{
			queue.push_back(vUp);
			m_PixelID.insert(vUp);
		}

		if (IsPixelOk(vDown))
		{
			queue.push_back(vDown);
			m_PixelID.insert(vDown);
		}

		if (IsPixelOk(vLeft))
		{
			queue.push_back(vLeft);
			m_PixelID.insert(vLeft);
		}

		if (IsPixelOk(vRight))
		{
			queue.push_back(vRight);
			m_PixelID.insert(vRight);
		}
	}

	m_MouseLT = ImVec2(left, top);
	m_MouseRB = ImVec2(right, bot);
}

bool SE_AtlasView::IsPixelOk(Vec2 _PixelPos)
{
	if (_PixelPos.x < 0 || m_AtlasTex->Width() <= _PixelPos.x || _PixelPos.y < 0 || m_AtlasTex->Height() <= _PixelPos.y)
	{
		return false;
	}

	// 이미 등록된 적 있는 Pixel이라면
	if (m_PixelID.end() != m_PixelID.find(_PixelPos))
		return false;

	// Pixel의 Alpha값이 0이라면
	int PixelIdx = m_AtlasTex->Width() * (int)_PixelPos.y + (int)_PixelPos.x;

	if (m_AtlasTex->GetPixels()[PixelIdx].a == 0)
		return false;

	return true;
}