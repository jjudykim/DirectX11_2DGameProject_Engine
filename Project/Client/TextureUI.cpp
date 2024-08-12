#include "pch.h"
#include "TextureUI.h"

#include <Engine/CTexture.h>

TextureUI::TextureUI()
	: AssetUI(ASSET_TYPE::TEXTURE)
{
}

TextureUI::~TextureUI()
{
}

void TextureUI::Update()
{
	Title();

	Ptr<CTexture> pTexture = (CTexture*)GetAsset().Get();

	// Image Frame
	ImVec2 frameSize = ImVec2(200.f, 200.f);

	// Image
	ImVec2 uv_min = ImVec2(0.0f, 0.0f);
	ImVec2 uv_max = ImVec2(1.0f, 1.0f);

	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 1.0f);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
	ImGui::Image(pTexture->GetSRV().Get(), ImVec2(180, 180), uv_min, uv_max, tint_col, border_col);

	// Texture Name
	OutputAssetName();

	// Resolution
	UINT width = pTexture->Width();
	UINT height = pTexture->Height();

	char buff[50] = {};
	sprintf_s(buff, "%d", width);
	ImGui::Text("Width");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureWidth", buff, 50, ImGuiInputTextFlags_ReadOnly);

	sprintf_s(buff, "%d", height);
	ImGui::Text("Height");
	ImGui::SameLine(100);
	ImGui::InputText("##TextureHeight", buff, 50, ImGuiInputTextFlags_ReadOnly);
}
