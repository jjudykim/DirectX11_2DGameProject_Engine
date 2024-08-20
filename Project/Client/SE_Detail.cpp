#include "pch.h"
#include "SE_Detail.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

#include "SE_AtlasView.h"

#include <Engine/CAsset.h>
#include <Engine/CSprite.h>
#include <Engine/CAssetMgr.h>

static char BufferForName[255] = "";

SE_Detail::SE_Detail()
	: m_Background(ImVec2(0, 0))
	, m_IsSelectedSprite(false)
	, m_IsRegistedSprite(false)
	, m_CurIndex(0)
{
}

SE_Detail::~SE_Detail()
{
}

void SE_Detail::Init()
{
	m_AtlasTex = nullptr;
	m_AtlasResolution = Vec2(0, 0);

	m_SpriteBundleName = "";
	m_SpriteName = "";
	m_CurSprite = nullptr;
	m_vecAddSprite.clear();

	m_CurIndex = 0;
	m_IsSelectedSprite = false;
	m_IsRegistedSprite = false;
	m_Background = ImVec2(0, 0);
}

void SE_Detail::Update()
{
	AtlasInfo();
	SpriteBundleInfo();
	SelectSpriteInfo();
	SpriteList();
}

void SE_Detail::AtlasInfo()
{
	ImGui::SeparatorText("Atlas Information");
	string TexName;

	if (m_AtlasTex != nullptr)
		TexName = string(m_AtlasTex->GetKey().begin(), m_AtlasTex->GetKey().end());

	ImGui::Text("Atlas Texture");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputText("##AtlasTex", (char*)TexName.c_str(), ImGuiInputTextFlags_::ImGuiInputTextFlags_ReadOnly);

	if (ImGui::BeginDragDropTarget())
	{
		const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("ContentTree");

		if (payload)
		{
			TreeNode** ppNode = (TreeNode**)payload->Data;
			TreeNode* pNode = *ppNode;

			Ptr<CAsset> pAsset = (CAsset*)pNode->GetData();
			if (ASSET_TYPE::TEXTURE == pAsset->GetAssetType())
			{
				SetAtlasTex((CTexture*)pAsset.Get());
			}
		}

		ImGui::EndDragDropTarget();
	}

	ImGui::SameLine();
	if (ImGui::Button("##AtlasTexBtn", ImVec2(18.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("Texture");
		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&SE_Detail::SelectTexture);
		pListUI->SetActive(true);
	}

	// Resolution
	// size[0] : 0, size[1] : 0
	int size[2] = { 0, 0 };

	if (m_AtlasTex != nullptr)
	{
		size[0] = m_AtlasTex->Width();
		size[1] = m_AtlasTex->Height();
		m_AtlasResolution = Vec2(size[0], size[1]);
	}

	ImGui::Text("Atlas Size");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputInt2("##TextureSize", size, ImGuiInputTextFlags_ReadOnly);

	ImGui::Text("");
}

void SE_Detail::SpriteBundleInfo()
{
	ImGui::SeparatorText("Sprite Bundle Information");

	string msg = "The names of sprite bundles and sprites are determined by the name of the specified \"Texture\".";
		   msg += "\nIf you need to modify it, please change the name of \"Texture\" in File Viewer.";

	// Bundle Name
	ImGui::Text("Bundle Name");
	ImGui::SameLine(120.f);
	if (m_AtlasTex != nullptr)
	{
		path Path = m_AtlasTex.Get()->GetKey();
		m_SpriteBundleName = Path.stem().string();
	}
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputText("##SpriteBundleName", (char*)m_SpriteBundleName.c_str(), ImGuiInputTextFlags_ReadOnly);

	// Name ToolTip
	ImGui::SameLine(0.f, 10.f);
	ImGui::TextDisabled("(?)");
	if (ImGui::BeginItemTooltip())
	{
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(msg.data());
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}

	int count = m_vecAddSprite.size();

	ImGui::Text("Sprite Count");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(90.f);
	ImGui::InputInt("##SpriteCount", &count, 0.f, 0.f, ImGuiInputTextFlags_ReadOnly);

	float background[2] = { m_Background.x, m_Background.y };
	ImGui::Text("Background");
	ImGui::SameLine(120.f);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputFloat2("##BackgroundSize", background, "%.2f");
	m_Background.x = background[0];
	m_Background.y = background[1];
	
	ImGui::Text("");
}

void SE_Detail::SelectSpriteInfo()
{
	ImGui::SeparatorText("Select Sprite");
	ImVec2 SpriteLT = GetAtlasView()->GetSpriteLT();
	ImVec2 SpriteRB = GetAtlasView()->GetSpriteRB();
	ImVec2 SpriteSize = ImVec2(SpriteRB.x - SpriteLT.x, SpriteRB.y - SpriteLT.y);

	// 빈 곳을 클릭한 경우 0,0 처리
	if (SpriteSize.x == 0 || SpriteSize.y == 0)
	{
		SpriteLT = ImVec2(0, 0);
		SpriteRB = ImVec2(0, 0);
		m_Background = ImVec2(0, 0);
		m_IsSelectedSprite = false;
	}
	else
	{
		if (m_CurSprite == nullptr)
		{
			m_CurSprite = new CSprite;
			Vec2 vSpriteLT = { SpriteLT.x, SpriteLT.y };
			Vec2 vSpriteSize = { SpriteSize.x, SpriteSize.y };

			m_CurSprite->Create(m_AtlasTex, vSpriteLT, vSpriteSize);
			CalcBackgroundSize();
			m_CurSprite->SetBackground(Vec2(m_Background.x, m_Background.y));
			m_IsSelectedSprite = false;
			memset(BufferForName, 0, 255);
		}
		else
		{
			if (IsChangedSelectSprite(SpriteLT, SpriteRB))
			{
				if (IsAddedSprite(m_CurSprite->GetID()) && m_IsSelectedSprite)
				{
					SpriteLT = ImVec2(m_CurSprite->GetLeftTopUV().x * m_AtlasResolution.x, m_CurSprite->GetLeftTopUV().y * m_AtlasResolution.y);
					SpriteRB = ImVec2((m_CurSprite->GetLeftTopUV().x + m_CurSprite->GetSliceUV().x) * m_AtlasResolution.x
						, (m_CurSprite->GetLeftTopUV().y + m_CurSprite->GetSliceUV().y) * m_AtlasResolution.y);
					SpriteSize = ImVec2(SpriteRB.x - SpriteLT.x, SpriteRB.y - SpriteLT.y);
					m_Background = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasResolution.x, m_CurSprite->GetBackgroundUV().y * m_AtlasResolution.y);
				}
				else
				{
					m_CurSprite = new CSprite;
					Vec2 vSpriteLT = { SpriteLT.x, SpriteLT.y };
					Vec2 vSpriteSize = { SpriteSize.x, SpriteSize.y };

					m_CurSprite->Create(m_AtlasTex, vSpriteLT, vSpriteSize);
					CalcBackgroundSize();
					m_CurSprite->SetBackground(Vec2(m_Background.x, m_Background.y));
					m_IsSelectedSprite = false;
					memset(BufferForName, 0, 255);
				}
			}
		}
	}

	// ==========================
	//        Sprite Name
	// ==========================
	// Temporary Sprite Name Set

	if (m_CurSprite != nullptr)
	{
		string Name = string(m_CurSprite->GetName().begin(), m_CurSprite->GetName().end());

		if (Name == "")
		{
			path Path = m_AtlasTex.Get()->GetKey();
			m_SpriteName = Path.stem().string();
			if (m_vecAddSprite.size() < 10)
				m_SpriteName += "_0" + std::to_string(m_vecAddSprite.size());
			else
				m_SpriteName += "_" + std::to_string(m_vecAddSprite.size());
		}
		else
		{
			m_SpriteName = Name;
		}

		if (IsAddedSprite(m_CurSprite->GetID()))
		{
			m_SpriteName = Name;
			strcpy_s(BufferForName, m_SpriteName.c_str());
		}
	}

	ImGui::Text("Sprite Name");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180);
	ImGui::InputTextWithHint("##SpriteName", (char*)m_SpriteName.c_str(), BufferForName, 255);


	// ==========================
	//       Sprite Detail
	// ==========================
	//  1. Position
	// --------------------------
	float pos[2] = { SpriteLT.x, SpriteLT.y };
	ImGui::Text("Position");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputFloat2("##SpritePos", pos, "%.2f", ImGuiInputTextFlags_ReadOnly);
	// --------------------------
	//  2. Size
	// --------------------------
	float size[2] = { SpriteSize.x, SpriteSize.y };
	ImGui::Text("Size");
	ImGui::SameLine(120);
	ImGui::SetNextItemWidth(180.f);
	ImGui::InputFloat2("##SpriteSize", size, "%.2f", ImGuiInputTextFlags_ReadOnly);
	// --------------------------
	//  3. Image
	// --------------------------
	ImVec2 frameSize = { 200.f, 200.f };
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);
	ImVec2 StartUV = ImVec2(0.f, 0.f);
	ImVec2 EndUV = ImVec2(1.f, 1.f);

	if (m_CurSprite != nullptr)
	{
		// Image size
		ImVec2 crop = ImVec2(m_CurSprite->GetBackgroundUV().x * m_AtlasResolution.x, m_CurSprite->GetBackgroundUV().y * m_AtlasResolution.y);

		if (frameSize.x != crop.x || frameSize.y != crop.y)
		{
			float ratio = crop.x / frameSize.x;
			ratio = 1 / ratio;

			crop.x *= ratio;
			crop.y *= ratio;
		}

		// Sprite Image
		ImVec2 StartPos = ImVec2((SpriteLT.x + (SpriteSize.x / 2.f)) - (m_Background.x / 2.f), (SpriteLT.y + (SpriteSize.y / 2.f)) - (m_Background.y / 2.f));
		StartUV = ImVec2(StartPos.x / m_AtlasResolution.x, StartPos.y / m_AtlasResolution.y);
		ImVec2 EndPos = ImVec2((SpriteRB.x - (SpriteSize.x / 2.f)) + (m_Background.x / 2.f), (SpriteRB.y - (SpriteSize.y / 2.f)) + (m_Background.y / 2.f));
		EndUV = ImVec2(EndPos.x / m_AtlasResolution.x, EndPos.y / m_AtlasResolution.y);
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - crop.x) * 0.5f);
		ImGui::Image(m_AtlasTex->GetSRV().Get(), crop, StartUV, EndUV, tint_col, border_col);
	}
	else
	{
		ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);
		ImGui::Image(nullptr, frameSize, StartUV, EndUV, tint_col, border_col);
	}
	
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) - 100.f);
	if (ImGui::Button("Add", ImVec2(100.f, 18.f)))
	{
		strcpy_s(BufferForName, m_SpriteName.c_str());
		ButtonAdd();
	}

	if (ImGui::IsKeyDown(ImGuiKey::ImGuiKey_LeftCtrl) && ImGui::IsKeyPressed(ImGuiKey::ImGuiKey_A))
	{
		strcpy_s(BufferForName, m_SpriteName.c_str());
		ButtonAdd();
	}

	ImGui::SameLine();
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) + 5.f);
	if (ImGui::Button("Delete", ImVec2(100.f, 18.f)))
	{
		vector<Ptr<CSprite>>::iterator iter = m_vecAddSprite.begin();

		for (; iter != m_vecAddSprite.end(); ++iter)
		{
			if (m_CurSprite == *iter)
			{
				m_vecAddSprite.erase(iter);
				break;
			}
		}
	}

	ImGui::TextDisabled("* Add ShrotCut : Ctrl + A");
	ImGui::Text("");
}

void SE_Detail::ButtonAdd()
{
	if (m_CurSprite != nullptr)
	{
		if (!IsAddedSprite(m_CurSprite->GetID()))
		{
			m_CurSprite->SetName(wstring(BufferForName, BufferForName + std::strlen(BufferForName)));
			m_CurSprite->SetBundleName(wstring(m_SpriteBundleName.begin(), m_SpriteBundleName.end()));

			m_vecAddSprite.push_back(m_CurSprite);
		}
		else
		{
			// 이미 등록된 Sprite라는 Message
		}
	}
}

void SE_Detail::SpriteList()
{
	ImGui::SeparatorText("Added Sprite List");

	if (ImGui::BeginListBox("##SE_AddedSpriteList", ImVec2(-FLT_MIN, 8 * ImGui::GetTextLineHeightWithSpacing())))
	{
		for (size_t i = 0; i < m_vecAddSprite.size(); i++)
		{
			const bool is_selected = (m_CurIndex == i);
			string label = string(m_vecAddSprite[i].Get()->GetName().begin(), m_vecAddSprite[i].Get()->GetName().end());
			if (ImGui::Selectable(label.c_str(), is_selected))
			{
				m_CurIndex = i;
				m_IsSelectedSprite = true;
			}

			// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
			if (is_selected)
			{
				ImGui::SetItemDefaultFocus();
				m_CurSprite = m_vecAddSprite[m_CurIndex];
			}
				
		}
		ImGui::EndListBox();
	}

	ImGui::SetCursorPosX((ImGui::GetWindowSize().x * 0.5f) + 5.f);
	if (m_vecAddSprite.size() > 0)
	{
		if (ImGui::Button("Register Sprites", ImVec2(150.f, 20.f)))
		{
			wstring BundlePath = CPathMgr::GetInst()->GetContentPath();
			wstring BundleName = wstring(m_SpriteBundleName.begin(), m_SpriteBundleName.end());
			BundlePath += L"sprite\\" + BundleName;

			struct stat st;
			if (stat(string(BundlePath.begin(), BundlePath.end()).c_str(), &st) != 0)
			{
				std::filesystem::create_directory(BundlePath);
			}
			
			for (size_t i = 0; i < m_vecAddSprite.size(); ++i)
			{
				wstring FilePath = BundlePath;
				FilePath += L"\\" + m_vecAddSprite[i]->GetName() + L".sprite";
				m_vecAddSprite[i]->Save(FilePath);
			}
			
			m_IsRegistedSprite = true;
		}
	}

	if (m_IsRegistedSprite)
	{
		ImGui::OpenPopup("All Sprites Registered");

		ImVec2 center = ImGui::GetMainViewport()->GetCenter();
		ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

		if (ImGui::BeginPopupModal("All Sprites Registered", NULL, ImGuiWindowFlags_AlwaysAutoResize))
		{
			ImGui::Text("All sprites are registered in the \"Content\"");
			ImGui::Separator();

			if (ImGui::Button("OK", ImVec2(120, 0)))
			{ 
				ImGui::CloseCurrentPopup(); 
				m_vecAddSprite.clear();
				m_IsRegistedSprite = false;
			}
			ImGui::EndPopup();
		}
	}
}

bool SE_Detail::IsChangedSelectSprite(Ptr<CSprite> _NewSprite)
{
	if (m_CurSprite->GetID() == _NewSprite->GetID())
		return false;

	else
		return true;
}


bool SE_Detail::IsChangedSelectSprite(ImVec2 _SpriteLT, ImVec2 _SpriteRB)
{
	Vec2 AtlasSize = Vec2(m_AtlasTex->Width(), m_AtlasTex->Height());
	Vec2 CurLT = m_CurSprite->GetLeftTopUV() * AtlasSize;
	Vec2 CurRB = (m_CurSprite->GetLeftTopUV() + m_CurSprite->GetSliceUV()) * AtlasSize;

	if (CurLT.x == _SpriteLT.x && CurLT.y == _SpriteLT.y)
		return false;

	if (CurRB.x == _SpriteRB.x && CurRB.y == _SpriteRB.y)
		return false;

	return true;
}

bool SE_Detail::IsAddedSprite(UINT _ID)
{
	for (size_t i = 0; i < m_vecAddSprite.size(); ++i)
	{
		if (_ID == m_vecAddSprite[i]->GetID())
			return true;
	}

	return false;
}

void SE_Detail::CalcBackgroundSize()
{
	// Background Size 갱신
	Vec2 vSpriteSize = m_CurSprite->GetSliceUV()* m_AtlasResolution;

	if (m_Background.x <= vSpriteSize.x || m_Background.y <= vSpriteSize.y)
	{
		if (vSpriteSize.x < 50.f && vSpriteSize.y < 50.f)
			m_Background = ImVec2(50.f, 50.f);
		else if (vSpriteSize.x < 100.f && vSpriteSize.y < 100.f)
			m_Background = ImVec2(100.f, 100.f);
		else if (vSpriteSize.x < 150.f && vSpriteSize.y < 150.f)
			m_Background = ImVec2(150.f, 150.f);
		else if (vSpriteSize.x < 200.f && vSpriteSize.y < 200.f)
			m_Background = ImVec2(200.f, 200.f);
		else if (vSpriteSize.x < 250.f && vSpriteSize.y < 250.f)
			m_Background = ImVec2(250.f, 250.f);
		else if (vSpriteSize.x < 300.f && vSpriteSize.y < 300.f)
			m_Background = ImVec2(300.f, 300.f);
		else if (vSpriteSize.x < 350.f && vSpriteSize.y < 350.f)
			m_Background = ImVec2(350.f, 350.f);
		else if (vSpriteSize.x < 400.f && vSpriteSize.y < 400.f)
			m_Background = ImVec2(400.f, 400.f);
		else if (vSpriteSize.x < 450.f && vSpriteSize.y < 450.f)
			m_Background = ImVec2(450.f, 450.f);
		else if (vSpriteSize.x < 500.f && vSpriteSize.y < 500.f)
			m_Background = ImVec2(500.f, 500.f);
		else if (vSpriteSize.x < 600.f && vSpriteSize.y < 600.f)
			m_Background = ImVec2(600.f, 600.f);
		else if (vSpriteSize.x < 700.f && vSpriteSize.y < 700.f)
			m_Background = ImVec2(700.f, 700.f);
		else if (vSpriteSize.x < 800.f && vSpriteSize.y < 800.f)
			m_Background = ImVec2(800.f, 800.f);
		else
			m_Background = ImVec2(1000.f, 1000.f);
	}
}

void SE_Detail::SetAtlasTex(Ptr<CTexture> _Tex)
{
	m_AtlasTex = _Tex;
	GetAtlasView()->SetAtlasTex(m_AtlasTex);
}

void SE_Detail::SelectTexture(DWORD_PTR _ListUI)
{
	ListUI* pListUI = (ListUI*)_ListUI;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_AtlasTex = nullptr;
		GetAtlasView()->SetAtlasTex(nullptr);
		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTex = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(pTex.Get());

	SetAtlasTex(pTex);
}
