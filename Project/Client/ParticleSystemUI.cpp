#include "pch.h"
#include "ParticleSystemUI.h"

#include "CEditorMgr.h"
#include "ListUI.h"
#include "TreeUI.h"

ParticleSystemUI::ParticleSystemUI()
	: ComponentUI(COMPONENT_TYPE::PARTICLESYSTEM)
{
}

ParticleSystemUI::~ParticleSystemUI()
{
}

void ParticleSystemUI::Update()
{
	if (GetTargetObject() == nullptr || GetTargetObject()->IsDead())
		return;

	m_UIHeight = 0;

	Title();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	m_Particle = GetTargetObject()->ParticleSystem();
	m_Module = m_Particle->GetParticleModule();

	ImGui::SeparatorText("Basic Setting");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	SetBasicOption();

	SetSpawn();
	SetSpawnBurst();
	SetVelocity();
	SetScale();
	SetDrag();
	SetNoiseForce();
	SetRender();

	m_UIHeight += 150;
	SetChildSize(ImVec2(0.f, (float)m_UIHeight));

	m_Particle->SetParticleModule(m_Module);
}

void ParticleSystemUI::SetBasicOption()
{
	// ==============================
	// Max Particle Count
	// ==============================
	m_ParticleCount = m_Particle->GetMaxParticleCount();
	ImGui::Text("Max Particle Count");
	ImGui::SameLine(140);
	ImGui::SetNextItemWidth(200);
	ImGui::InputInt("##ParticleCount", &m_ParticleCount, 0, 0);
	m_Particle->SetMaxParticleCount(m_ParticleCount);
	m_UIHeight += (int)ImGui::GetItemRectSize().y;


	// ==============================
	// Particle Texture
	// ==============================
	m_ParticleTexture = m_Particle->GetParticleTexture();
	ImGui::Text("Particle Texture");
	ImGui::SameLine(140);
	ImGui::SetNextItemWidth(175);

	char cParticleName[256] = {};
	if (m_ParticleTexture != nullptr)
	{
		string strParticleName = string(m_ParticleTexture->GetKey().begin(), m_ParticleTexture->GetKey().end());
		strcpy_s(cParticleName, strParticleName.c_str());
	}

	ImGui::InputText("##ParticleTextureName", cParticleName, ImGuiInputTextFlags_ReadOnly);
	ImGui::SameLine(0, 5.f);

	if (ImGui::Button("##TextureChangeButton", ImVec2(20.f, 18.f)))
	{
		ListUI* pListUI = (ListUI*)CEditorMgr::GetInst()->FindEditorUI("ListUI");
		pListUI->SetName("Texture");

		vector<string> vecTexNames;
		CAssetMgr::GetInst()->GetAssetNames(ASSET_TYPE::TEXTURE, vecTexNames);
		pListUI->AddList(vecTexNames);
		pListUI->AddDelegate(this, (DELEGATE_1)&ParticleSystemUI::ChangeTexture);
		pListUI->SetActive(true);
	}
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImVec2 frameSize = ImVec2(200.f, 200.f);

	ImVec2 uv_min = ImVec2(0.f, 0.f);
	ImVec2 uv_max = ImVec2(1.f, 1.f);
	ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f);
	ImVec4 border_col = ImVec4(0.7f, 0.7f, 0.7f, 0.7f);
	ImGui::SetCursorPosX((ImGui::GetWindowSize().x - frameSize.x) * 0.5f);

	if (m_ParticleTexture != nullptr)
		ImGui::Image(m_ParticleTexture->GetSRV().Get(), ImVec2(180, 180), uv_min, uv_max, tint_col, border_col);
	else
		ImGui::Image(nullptr, ImVec2(180, 180), uv_min, uv_max, tint_col, border_col);

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

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
				m_ParticleTexture = ((CTexture*)pAsset.Get());
			}
		}
		ImGui::EndDragDropTarget();
	}
	m_Particle->SetParticleTexture(m_ParticleTexture);


	// ==============================
	// Particle Module On/Off
	// ==============================
	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	ImGui::SeparatorText("Particle Module On / Off");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	const int ModuleCount = (UINT)PARTICLE_MODULE::END;

	bool bModule[ModuleCount] = {};
	string strArrMoudleName[ModuleCount] = { "Spawn", "Spawn Burst", "Add Velocity", "Scale", "Drag", "Noise Force", "Render" };

	for (int i = 0; i < ModuleCount; ++i)
	{
		bModule[i] = m_Module.Module[i];
	}

	int ChangedIdx = -1;
	ImVec2 tableSize = ImVec2(ImGui::GetWindowSize().x - 2.f, 0.f);
	if (ImGui::BeginTable("##ModuleCheckTable", 2, ImGuiTableFlags_SizingStretchSame | ImGuiTableFlags_Resizable | ImGuiTableFlags_Borders, tableSize))
	{
		for (int i = 0; i < ModuleCount; i++)
		{
			char label[256];
			sprintf_s(label, "Module %d", i);
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			if (ImGui::Selectable(label, &bModule[i], ImGuiSelectableFlags_SpanAllColumns))
			{
				ChangedIdx = i;
			}

			string strModuleName = strArrMoudleName[i];
			ImGui::TableNextColumn();
			ImGui::Text(strModuleName.c_str());
		}
		ImGui::EndTable();
	}

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (ChangedIdx != -1)
	{
		m_Module.Module[ChangedIdx] = bModule[ChangedIdx];
	}

	ImGui::Text("");
	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetSpawn()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Spawn"))
	{
		// Spawn Rate
		ImGui::Text("Spawn Rate");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		int uSpawnRate = m_Module.SpawnRate;
		ImGui::DragInt("##Spawn_Rate", &uSpawnRate, 0, 0);
		m_Module.SpawnRate = (UINT)uSpawnRate;
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Spawn Color
		ImGui::Text("Spawn Color");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		float fColor[4] = { m_Module.vSpawnColor.x, m_Module.vSpawnColor.y, m_Module.vSpawnColor.z, m_Module.vSpawnColor.w };
		ImGui::ColorEdit4("##Spawn_Color", fColor);
		m_Module.vSpawnColor.x = fColor[0];
		m_Module.vSpawnColor.y = fColor[1];
		m_Module.vSpawnColor.z = fColor[2];
		m_Module.vSpawnColor.w = fColor[3];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Spawn Min Scale
		ImGui::Text("Spawn Min Scale");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		float fMinScale[4] = { m_Module.vSpawnMinScale.x, m_Module.vSpawnMinScale.y, m_Module.vSpawnMinScale.z, m_Module.vSpawnMinScale.w };
		ImGui::DragFloat4("##Spawn_MinScale", fMinScale, 1.0f, 0.f, 0.f, "%.1f");
		m_Module.vSpawnMinScale.x = fMinScale[0];
		m_Module.vSpawnMinScale.y = fMinScale[1];
		m_Module.vSpawnMinScale.z = fMinScale[2];
		m_Module.vSpawnMinScale.w = fMinScale[3];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Spawn Max Scale
		ImGui::Text("Spawn Max Scale");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		float fMaxScale[4] = { m_Module.vSpawnMaxScale.x, m_Module.vSpawnMaxScale.y, m_Module.vSpawnMaxScale.z, m_Module.vSpawnMaxScale.w };
		ImGui::DragFloat4("##Spawn_MaxScale", fMaxScale, 1.0f, 0.f, 0.f, "%.1f");
		m_Module.vSpawnMaxScale.x = fMaxScale[0];
		m_Module.vSpawnMaxScale.y = fMaxScale[1];
		m_Module.vSpawnMaxScale.z = fMaxScale[2];
		m_Module.vSpawnMaxScale.w = fMaxScale[3];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		ImGui::Separator();
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Min Life
		ImGui::Text("Min Life");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Spawn_MinLife", &m_Module.MinLife, 1.0f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Max Life
		ImGui::Text("Max Life");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Spawn_MaxLife", &m_Module.MaxLife, 1.0f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		ImGui::Separator();
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Spawn Shape
		ImGui::Text("Spawn Shape");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(70);
		{
			UINT uShape = m_Module.SpawnShape;

			const char* items[] = { "Box", "Sphere" };
			const char* combo_preview_value = items[uShape];

			if (ImGui::BeginCombo("##Spawn_Shape", combo_preview_value))
			{
				for (int i = 0; i < 2; i++)
				{
					const bool is_selected = (uShape == i);

					if (ImGui::Selectable(items[i], is_selected))
					{
						uShape = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
			m_Module.SpawnShape = uShape;
		}


		// Spawn Scale
		ImGui::SameLine(0, 5);
		ImGui::SetNextItemWidth(125);
		float fShapeScale[3] = { m_Module.SpawnShapeScale.x, m_Module.SpawnShapeScale.y, m_Module.SpawnShapeScale.z };
		ImGui::DragFloat3("##Spawn_ShapeScale", fShapeScale, 1.0f, 0.f, 0.f, "%.1f");
		m_Module.SpawnShapeScale.x = fShapeScale[0];
		m_Module.SpawnShapeScale.y = fShapeScale[1];
		m_Module.SpawnShapeScale.z = fShapeScale[2];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Block Spawn Shape
		ImGui::Text("Block Spawn Shape");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(70);
		{
			UINT uShape = m_Module.BlockSpawnShape;

			const char* items[] = { "Box", "Sphere" };
			const char* combo_preview_value = items[uShape];

			if (ImGui::BeginCombo("##Spawn_BlockShape", combo_preview_value))
			{
				for (int i = 0; i < 2; i++)
				{
					const bool is_selected = (uShape == i);

					if (ImGui::Selectable(items[i], is_selected))
					{
						uShape = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
			m_Module.BlockSpawnShape = uShape;
		}

		// Block Spawn Shape Scale
		ImGui::SameLine(0, 5);
		ImGui::SetNextItemWidth(125);
		float fBlockSpawnShapeScale[3] = { m_Module.BlockSpawnShapeScale.x, m_Module.BlockSpawnShapeScale.y, m_Module.BlockSpawnShapeScale.z };
		ImGui::DragFloat3("##Spawn_BlockSpawnShapeScale", fBlockSpawnShapeScale, 1.0f, 0.f, 0.f, "%.1f");
		m_Module.BlockSpawnShapeScale.x = fBlockSpawnShapeScale[0];
		m_Module.BlockSpawnShapeScale.y = fBlockSpawnShapeScale[1];
		m_Module.BlockSpawnShapeScale.z = fBlockSpawnShapeScale[2];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		ImGui::Separator();
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Space Type
		ImGui::Text("Space Type");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		{
			UINT uType = m_Module.SpaceType;

			const char* items[] = { "Local", "World" };
			const char* combo_preview_value = items[uType];

			if (ImGui::BeginCombo("##Spawn_SpaceType", combo_preview_value))
			{
				for (int i = 0; i < 2; i++)
				{
					const bool is_selected = (uType == i);

					if (ImGui::Selectable(items[i], is_selected))
					{
						uType = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
			m_Module.SpaceType = uType;
		}
	}

	m_UIHeight += (int)ImGui::GetItemRectSize().y;

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] == 0)
		ImGui::EndDisabled();
}

void ParticleSystemUI::SetSpawnBurst()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Spawn Burst"))
	{
		// Spawn Burst Count
		ImGui::Text("Spawn Burst Count");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		int iSpawnBurstCount = m_Module.SpawnBurstCount;
		ImGui::DragInt("##SpawnBurst_Count", &iSpawnBurstCount, 1.0f, 0.f, 0.f);
		m_Module.SpawnBurstCount = (UINT)iSpawnBurstCount;
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		// Spawn Burst Repeat
		ImGui::Text("Spawn Burst Repeat");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		int iSpawnBurstRepeat = m_Module.SpawnBurstRepeat;
		ImGui::DragInt("##SpawnBurst_Repeat", &iSpawnBurstRepeat, 1.0f, 0.f, 0.f);
		m_Module.SpawnBurstRepeat = (UINT)iSpawnBurstRepeat;
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		// Spawn Burst Repeat Time
		ImGui::Text("Spawn Burst Re-Time");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##SpawnBurst_RepeatTime", &m_Module.SpawnBurstRepeatTime, 1.0f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetVelocity()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Add Velocity"))
	{
		// Add Velocity Type  (0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed)
		ImGui::Text("Add Velocity Type");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		{
			UINT uType = m_Module.AddVelocityType;

			const char* items[] = { "Random", "From Center", "To Center", "Fixed" };
			const char* combo_preview_value = items[uType];

			if (ImGui::BeginCombo("##AddVelocity_Type", combo_preview_value))
			{
				for (int i = 0; i < 4; i++)
				{
					const bool is_selected = (uType == i);

					if (ImGui::Selectable(items[i], is_selected))
					{
						uType = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
			m_Module.AddVelocityType = uType;
		}


		// Add Velocity Fixed Dir
		ImGui::Text("Add Velocity Dir");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		float fVelocityDir[3] = { m_Module.AddVelocityFixedDir.x, m_Module.AddVelocityFixedDir.y, m_Module.AddVelocityFixedDir.z };
		ImGui::DragFloat3("##AddVelocity_Dir", fVelocityDir, 1.0f, 0.f, 0.f, "%.1f");
		m_Module.AddVelocityFixedDir.x = fVelocityDir[0];
		m_Module.AddVelocityFixedDir.y = fVelocityDir[1];
		m_Module.AddVelocityFixedDir.z = fVelocityDir[2];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		ImGui::Separator();
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		// Add Min Speed
		ImGui::Text("Add Min Speed");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##AddVelocity_Min", &m_Module.AddMinSpeed, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Add Max Speed
		ImGui::Text("Add Max Speed");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##AddVelocity_Max", &m_Module.AddMaxSpeed, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetScale()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Scale"))
	{
		// Start Scale
		ImGui::Text("Start Scale");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Scale_Start", &m_Module.StartScale, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// End Scale
		ImGui::Text("End Scale");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Scale_End", &m_Module.EndScale, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetDrag()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Drag"))
	{
		// DestNormalizedAge;
		ImGui::Text("Dest Normalized Age");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Drag_NormalizedAge", &m_Module.DestNormalizedAge, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Limit Speed;
		ImGui::Text("Limit Speed");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Drag_LimitSpeed", &m_Module.LimitSpeed, 1.0f, 0.f, 0.f, "%.1f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetNoiseForce()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Noise Force"))
	{
		// Noise Force Term
		ImGui::Text("Noise Force Term");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Noise_ForceTerm", &m_Module.NoiseForceTerm, 1.0f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Noise Force Scale
		ImGui::Text("Noise Force Scale");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Noise_ForceScale", &m_Module.NoiseForceScale, 1.0f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}

void ParticleSystemUI::SetRender()
{
	if (m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] == 0)
		ImGui::BeginDisabled();

	if (ImGui::CollapsingHeader("Render"))
	{
		// End Color
		ImGui::Text("End Color");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		float fEndColor[3] = { m_Module.EndColor.x, m_Module.EndColor.y, m_Module.EndColor.z };
		ImGui::ColorEdit3("##Render_EndColor", fEndColor);
		m_Module.EndColor.x = fEndColor[0];
		m_Module.EndColor.y = fEndColor[1];
		m_Module.EndColor.z = fEndColor[2];
		m_UIHeight += (int)ImGui::GetItemRectSize().y;

		// Fade Out
		ImGui::Text("Fade Out");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		{
			UINT uType = m_Module.FadeOut;

			const char* items[] = { "Off", "Normalized Age" };
			const char* combo_preview_value = items[uType];

			if (ImGui::BeginCombo("##Render_FadeOut", combo_preview_value))
			{
				for (int i = 0; i < 2; i++)
				{
					const bool is_selected = (uType == i);

					if (ImGui::Selectable(items[i], is_selected))
					{
						uType = i;
					}

					// Set the initial focus when opening the combo (scrolling + keyboard navigation focus)
					if (is_selected)
						ImGui::SetItemDefaultFocus();
				}
				ImGui::EndCombo();
			}
			m_UIHeight += (int)ImGui::GetItemRectSize().y;
			m_Module.FadeOut = uType;
		}


		// FadeOut Start Ratio
		ImGui::Text("FadeOut Start Ratio");
		ImGui::SameLine(140);
		ImGui::SetNextItemWidth(200);
		ImGui::DragFloat("##Render_FOStartRatio", &m_Module.FadeOutStartRatio, 0.1f, 0.f, 0.f, "%.2f");
		m_UIHeight += (int)ImGui::GetItemRectSize().y;


		// Velocity Alignment
		ImGui::Text("Fade Out");
		ImGui::SameLine(140);
		bool isVelocityAlignmentOn = m_Module.VelocityAlignment == 0 ? false : true;
		ImGui::Checkbox("##Render_VelocityAlignment", &isVelocityAlignmentOn);
		m_Module.VelocityAlignment = isVelocityAlignmentOn == true ? 1 : 0;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] == 0)
		ImGui::EndDisabled();

	m_UIHeight += (int)ImGui::GetItemRectSize().y;
}


void ParticleSystemUI::ChangeTexture(DWORD_PTR param)
{
	ListUI* pListUI = (ListUI*)param;
	string strName = pListUI->GetSelectName();

	if (strName == "None")
	{
		m_Particle->SetParticleTexture(nullptr);

		return;
	}

	wstring strAssetName = wstring(strName.begin(), strName.end());

	Ptr<CTexture> pTexture = CAssetMgr::GetInst()->FindAsset<CTexture>(strAssetName);

	assert(m_Particle);

	m_Particle->SetParticleTexture(pTexture);
}
