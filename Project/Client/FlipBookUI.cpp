#include "pch.h"
#include "FlipBookUI.h"

FlipBookUI::FlipBookUI()
	: AssetUI(ASSET_TYPE::FLIPBOOK)
	, m_CurFrmIdx(0)
	, m_AccTime(0)
	, m_FrameSize(300.f, 300.f)
	, m_CurFB(nullptr)
	, m_CurSprite(nullptr)
	, m_CurAtlas(nullptr)
	, m_Repeat(false)
{
}

FlipBookUI::~FlipBookUI()
{
}

void FlipBookUI::Init()
{
	m_CurFrmIdx = 0;
	m_AccTime = 0;
	m_FrameSize = ImVec2(300.f, 300.f);

	m_CurFB = (CFlipBook*)GetAsset().Get();
	m_CurSprite = m_CurFB->GetSprite(m_CurFrmIdx);
	m_CurAtlas = m_CurSprite->GetAtlasTexture();
}

void FlipBookUI::Update()
{
	if (m_CurFB == nullptr || m_CurFB->GetID() != GetAsset()->GetID())
		Init();

	Title();
	CurFlipBookInfo();
	ShowFlipBookSprite();
}

void FlipBookUI::CurFlipBookInfo()
{
	ImGui::SeparatorText("FlipBook Information");

	ImGui::Text("FlipBook Name");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(230.f);
	string Key;
	if (m_CurFB.Get() == nullptr)
		Key = "None";
	else
		Key = string(m_CurFB.Get()->GetKey().begin(), m_CurFB.Get()->GetKey().end());
	ImGui::InputText("##FlipBookKey", (char*)Key.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	// Current Frame Index / Sprite
	ImGui::Text("Current Frame");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(30.f);
	ImGui::InputInt("##FlipBookFrmIdx", (int*)&m_CurFrmIdx, 0, 0, ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine(0.f, 5.f);
	ImGui::SetNextItemWidth(150.f);
	path Path = m_CurSprite.Get()->GetKey();
	string spriteKey = Path.stem().string();
	ImGui::InputText("##SpriteKey", spriteKey.data(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	ImGui::SameLine(0.f, 5.f);
	ImGui::PushButtonRepeat(true);
	if (ImGui::ArrowButton("##left", ImGuiDir_Left))
	{
		if (m_CurFrmIdx == 0)
			m_CurFrmIdx = 0;
		else
			m_CurFrmIdx--;
	}
	ImGui::SameLine(0.0f, 5.f);
	if (ImGui::ArrowButton("##right", ImGuiDir_Right))
	{
		if (m_CurFrmIdx >= m_CurFB->GetMaxFrameCount() - 1)
			m_CurFrmIdx = m_CurFB->GetMaxFrameCount() - 1;
		else
			m_CurFrmIdx++;
	}
	ImGui::PopButtonRepeat();


	// FPS
	ImGui::Text("FPS");
	float FPS = m_CurFB->GetFPS();
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(120);
	ImGui::InputFloat("##FlipBookFPS", &FPS, 0, 0, "%.2f", ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	// Repeat
	ImGui::Text("Repeat");
	ImGui::SameLine(120);
	ImGui::Checkbox("##FlipBookRepeat", &m_Repeat);

	ImGui::Text("");
}

void FlipBookUI::ShowFlipBookSprite()
{
	ImGui::SeparatorText("FlipBook Player");

	if (m_Playing)
	{
		m_AccTime += EngineDT;
	}

	float MaxTime = 1.f / m_CurFB->GetFPS();
	if (MaxTime < m_AccTime)
	{
		m_AccTime -= MaxTime;
		++m_CurFrmIdx;

		if (m_CurFB->GetMaxFrameCount() <= m_CurFrmIdx)
		{
			m_CurFrmIdx = 0;
			if (!m_Repeat)
				m_Playing = false;
		}
	}

	m_CurSprite = m_CurFB->GetSprite(m_CurFrmIdx);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);

	// Viewer Frame
	if (m_CurSprite != nullptr)
	{
		ImVec2 OffsetUV = ImVec2(m_CurSprite->GetOffsetUV().x, m_CurSprite->GetOffsetUV().y);
		ImVec2 StartUV = ImVec2(m_CurSprite->GetLeftTopUV().x - OffsetUV.x, m_CurSprite->GetLeftTopUV().y - OffsetUV.y);
		ImVec2 SliceUV = ImVec2(m_CurSprite->GetSliceUV().x, m_CurSprite->GetSliceUV().y);
		ImVec2 BackgroundUV = ImVec2(m_CurSprite->GetBackgroundUV().x, m_CurSprite->GetBackgroundUV().y);
		ImVec2 EndUV = ImVec2(StartUV.x + SliceUV.x, StartUV.y + SliceUV.y);

		ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * m_CurAtlas->Width(), m_CurSprite->GetBackgroundUV().y * m_CurAtlas->Height());

		if (m_FrameSize.x != crop.x || m_FrameSize.y != crop.y)
		{
			float ratio = crop.x / m_FrameSize.x;
			ratio = 1 / ratio;

			crop.x *= ratio;
			crop.y *= ratio;
		}

		StartUV = ImVec2((StartUV.x + (SliceUV.x / 2.f)) - (BackgroundUV.x / 2.f)
			           , (StartUV.y + (SliceUV.y / 2.f)) - (BackgroundUV.y / 2.f));
		EndUV = ImVec2(StartUV.x + BackgroundUV.x, StartUV.y + BackgroundUV.y);

		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - m_FrameSize.x) * 0.5f);
		ImGui::Image(m_CurAtlas->GetSRV().Get(), crop, StartUV, EndUV, tint_col, border_col);
	}
	else
	{
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - m_FrameSize.x) * 0.5f);
		ImGui::Image(ImTextureID(0), m_FrameSize, ImVec2(0.0f, 0.0f), ImVec2(1.0f, 1.0f), tint_col, border_col);
	}

	ImGui::Text("");

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("PLAY", ImVec2(95.f, 18.f)))
	{
		m_Playing = true;
	}

	ImGui::SameLine((ImGui::GetWindowSize().x * 0.5f) + 5.f);
	if (ImGui::Button("STOP", ImVec2(95.f, 18.f)))
	{
		m_Playing = false;
		m_CurFrmIdx = 0;
		m_AccTime = 0;
	}
}