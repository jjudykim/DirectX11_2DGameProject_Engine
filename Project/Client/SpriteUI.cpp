#include "pch.h"
#include "SpriteUI.h"

#include "TreeUI.h"

SpriteUI::SpriteUI()
	: AssetUI(ASSET_TYPE::SPRITE)
{
}

SpriteUI::~SpriteUI()
{
}

void SpriteUI::Update()
{
	Title();

	SpriteInfo();
	AtlasInfo();
}

void SpriteUI::SpriteInfo()
{
	if (m_CurSprite == nullptr)
	{
		m_CurSprite = (CSprite*)GetAsset().Get();
		m_AtlasTex = m_CurSprite->GetAtlasTexture();
		SetSpriteInfo();
	}
	else
	{
		if (m_CurSprite->GetID() != GetAsset()->GetID())
		{
			m_CurSprite = (CSprite*)GetAsset().Get();
			m_AtlasTex = m_CurSprite->GetAtlasTexture();
			SetSpriteInfo();
		}
	}
	string Name = string(m_CurSprite->GetKey().begin(), m_CurSprite->GetKey().end());

	ImGui::Text("Sprite Name");
	ImGui::SameLine(100);
	ImGui::InputText("##AssetName", (char*)Name.c_str(), Name.length(), ImGuiInputTextFlags_ReadOnly);

	// Image size
	ImVec2 image = m_SpriteBG;

	ImVec2 frameSize = { 200.f, 200.f };
	if (frameSize.x != image.x || frameSize.y != image.y)
	{
		float ratio = image.x / frameSize.x;
		ratio = 1 / ratio;

		image.x *= ratio;
		image.y *= ratio;
	}

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);

	ImVec2 StartPos = ImVec2((m_SpriteLT.x + (m_SpriteSlice.x / 2.f)) - (m_SpriteBG.x / 2.f), (m_SpriteLT.y + (m_SpriteSlice.y / 2.f)) - (m_SpriteBG.y / 2.f));
	ImVec2 StartUV = ImVec2(StartPos.x / m_AtlasSize.x, StartPos.y / m_AtlasSize.y);
	ImVec2 EndPos = ImVec2(StartPos.x + m_SpriteBG.x, StartPos.y + m_SpriteBG.y);
	ImVec2 EndUV = ImVec2(EndPos.x / m_AtlasSize.x, EndPos.y / m_AtlasSize.y);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - image.x) * 0.5f);
	ImGui::Image(m_AtlasTex->GetSRV().Get(), image, StartUV, EndUV, tint_col, border_col);

	ImGui::Text("");
	ImGui::SeparatorText("Sprite Information");

	// Position
	ImGui::Text("Position");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpritePosX", &m_SpriteLT.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpritePosY", &m_SpriteLT.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

	// Size
	ImGui::Text("Size");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpriteSizeX", &m_SpriteSlice.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##SpriteSizeY", &m_SpriteSlice.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);

	// Background
	ImGui::Text("Background");
	ImGui::SameLine(100);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##BackgroundX", &m_SpriteBG.x, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(205.f);
	ImGui::SetNextItemWidth(100.f);
	ImGui::InputFloat("##BackgroundY", &m_SpriteBG.y, 0, 0, "%.3f", ImGuiInputTextFlags_ReadOnly);
}

void SpriteUI::SetSpriteInfo()
{
	m_AtlasSize = ImVec2(m_AtlasTex->Width(), m_AtlasTex->Height());

	m_SpriteLT = ImVec2(m_CurSprite->GetLeftTopUV().x * m_AtlasSize.x, m_CurSprite->GetLeftTopUV().y * m_AtlasSize.y);
	m_SpriteSlice = ImVec2(m_CurSprite->GetSliceUV().x * m_AtlasSize.x, m_CurSprite->GetSliceUV().y * m_AtlasSize.y);
	m_SpriteBG = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasSize.x, m_CurSprite->GetBackgroundUV().y * m_AtlasSize.y);
}

void SpriteUI::AtlasInfo()
{
	ImGui::Text("");
	ImGui::SeparatorText("Atlas Information");

	string TexName;

	if (m_AtlasTex != nullptr)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(100);
	ImGui::InputText("##AtlasTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);


}