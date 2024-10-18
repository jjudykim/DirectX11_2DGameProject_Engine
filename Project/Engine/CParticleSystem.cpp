#include "pch.h"
#include "CParticleSystem.h"

#include "CTimeMgr.h"

#include "CDevice.h"
#include "CAssetMgr.h"
#include "CStructuredBuffer.h"

#include "CTransform.h"

CParticleSystem::CParticleSystem()
	: CRenderComponent(COMPONENT_TYPE::PARTICLESYSTEM)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_Time(0.f)
	, m_MaxParticleCount(1000)
{
	// Mesh / Material
	SetMesh(CAssetMgr::GetInst()->FindAsset<CMesh>(L"PointMesh"));
	SetMaterial(CAssetMgr::GetInst()->FindAsset<CMaterial>(L"ParticleRenderMtrl"));

	// ParticleTick ComputeShader
	m_TickCS = (CParticleTickCS*)CAssetMgr::GetInst()->FindAsset<CComputeShader>(L"ParticleTickCS").Get();

	m_ParticleTex = CAssetMgr::GetInst()->FindAsset<CTexture>(L"texture\\particle\\Smoke_Cartoon.png");

	m_ParticleBuffer = new CStructuredBuffer;
	m_ParticleBuffer->Create(sizeof(tParticle), m_MaxParticleCount, SB_TYPE::SRV_UAV);

	m_SpawnCountBuffer = new CStructuredBuffer;
	m_SpawnCountBuffer->Create(sizeof(tSpawnCount), 1, SB_TYPE::SRV_UAV, true, nullptr);


	// 파티클 기능(모듈) 정보 세팅
	// Spawn Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN] = true;
	m_Module.SpawnRate = 20;
	m_Module.vSpawnColor = Vec4(0.24f, 0.67f, 0.87f, 1.f);
	m_Module.MinLife = 1.f;
	m_Module.MaxLife = 5.f;
	m_Module.vSpawnMinScale = Vec3(50.f, 5.f, 1.f);
	m_Module.vSpawnMaxScale = Vec3(500.f, 50.f, 1.f);

	m_Module.SpawnShape = 1;
	m_Module.SpawnShapeScale.x = 500.f;

	m_Module.BlockSpawnShape = 1;
	m_Module.BlockSpawnShapeScale.x = 0.f;

	m_Module.SpaceType = 1; // Local Space 

	// Spawn Burst Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST] = true;
	m_Module.SpawnBurstRepeat = true;
	m_Module.SpawnBurstCount = 100;
	m_Module.SpawnBurstRepeatTime = 3.f;

	// Scale Module
	m_Module.Module[(UINT)PARTICLE_MODULE::SCALE] = true;
	m_Module.StartScale = 1.f;
	m_Module.EndScale = 1.f;

	// AddVelocity Module
	m_Module.Module[(UINT)PARTICLE_MODULE::ADD_VELOCITY] = true;
	m_Module.AddVelocityType = 1;
	m_Module.AddVelocityFixedDir = Vec3(0.f, 1.f, 0.f);
	m_Module.AddMinSpeed = 100.f;
	m_Module.AddMaxSpeed = 500.f;

	// Drag Module (감속)
	m_Module.Module[(UINT)PARTICLE_MODULE::DRAG] = false;
	m_Module.DestNormalizedAge = 1.f;
	m_Module.LimitSpeed = 0.f;

	// Noise Force Module
	m_Module.Module[(UINT)PARTICLE_MODULE::NOISE_FORCE] = false;
	m_Module.NoiseForceTerm = 0.3f;
	m_Module.NoiseForceScale = 100.f;

	// Render Module
	m_Module.Module[(UINT)PARTICLE_MODULE::RENDER] = true;
	m_Module.EndColor = Vec3(1.f, 0.f, 0.f);
	m_Module.FadeOut = true;
	m_Module.FadeOutStartRatio = 0.9f;
	m_Module.VelocityAlignment = true;

	m_ModuleBuffer = new CStructuredBuffer;
	m_ModuleBuffer->Create(sizeof(tParticleModule), 1, SB_TYPE::SRV_UAV, true, &m_Module);
}

CParticleSystem::CParticleSystem(const CParticleSystem& _Other)
	: CRenderComponent(_Other)
	, m_TickCS(_Other.m_TickCS)
	, m_ParticleBuffer(nullptr)
	, m_SpawnCountBuffer(nullptr)
	, m_ModuleBuffer(nullptr)
	, m_ParticleTex(_Other.m_ParticleTex)
	, m_Time(0.f)
	, m_BurstTime(0.f)
	, m_MaxParticleCount(_Other.m_MaxParticleCount)
	, m_Module(_Other.m_Module)
{
	assert(_Other.m_ParticleBuffer && _Other.m_SpawnCountBuffer && _Other.m_ModuleBuffer);

	m_ParticleBuffer = new CStructuredBuffer(*_Other.m_ParticleBuffer);
	m_SpawnCountBuffer = new CStructuredBuffer(*_Other.m_SpawnCountBuffer);
	m_ModuleBuffer = new CStructuredBuffer(*_Other.m_ModuleBuffer);
}

CParticleSystem::~CParticleSystem()
{
	DELETE(m_ParticleBuffer);
	DELETE(m_SpawnCountBuffer);
	DELETE(m_ModuleBuffer);
}

void CParticleSystem::FinalTick()
{
	// SpawnCount 계산
	CalculateSpawnCount();

	// ComputeShader
	m_TickCS->SetParticleWorldPos(Transform()->GetWorldPos());
	m_TickCS->SetParticleBuffer(m_ParticleBuffer);
	m_TickCS->SetSpawnCount(m_SpawnCountBuffer);
	m_TickCS->SetModuleBuffer(m_ModuleBuffer);

	m_TickCS->Execute();
}

void CParticleSystem::Render()
{
	// Module Update
	if (m_ModuleBuffer != nullptr)
		m_ModuleBuffer->SetData(&m_Module, sizeof(tParticleModule));

	// 위치 정보 바인딩
	Transform()->Binding();

	// 파티클 버퍼 바인딩
	m_ParticleBuffer->Binding(30);

	// 모듈 버퍼 바인딩
	m_ModuleBuffer->Binding(31);

	GetMaterial()->SetTexParam(TEX_0, m_ParticleTex);
	GetMaterial()->Binding();

	GetMesh()->Render_Particle(m_MaxParticleCount);

	m_ParticleBuffer->Clear(30);
	m_ModuleBuffer->Clear(31);
}

void CParticleSystem::CalculateSpawnCount()
{
	m_Time += EngineDT;
	tSpawnCount count = {  };

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN])
	{
		// SpawnRate 에 따른 이번 Tick 에서 생성시킬 파티클의 수
		float Term = 1.f / (float)m_Module.SpawnRate;
		UINT SpawnCount = 0;

		if (Term < m_Time)
		{
			float Value = m_Time / Term;
			SpawnCount = (UINT)Value;
			m_Time -= (float)SpawnCount * Term;
		}

		count.SpawnCount = SpawnCount;
	}

	if (m_Module.Module[(UINT)PARTICLE_MODULE::SPAWN_BURST])
	{
		UINT BurstCount = 0;

		if (0.f == m_BurstTime)
		{
			BurstCount = m_Module.SpawnBurstCount;
		}

		m_BurstTime += EngineDT;

		if (m_Module.SpawnBurstRepeat && m_Module.SpawnBurstRepeatTime <= m_BurstTime)
		{
			m_BurstTime -= m_Module.SpawnBurstRepeatTime;
			BurstCount += m_Module.SpawnBurstCount;
		}

		count.SpawnCount += BurstCount;
	}

	// SpawnCount 를 Buffer 에 전달	
	m_SpawnCountBuffer->SetData(&count);
}

void CParticleSystem::SaveToFile(FILE* _File)
{
	// Max Particle Count
	fwrite(&m_MaxParticleCount, sizeof(int), 1, _File);

	// Paticle Texture
	SaveAssetRef(m_ParticleTex, _File);

	// Module
	// Spawn
	fwrite(&m_Module.SpawnRate, sizeof(UINT), 1, _File);
	fwrite(&m_Module.vSpawnColor, sizeof(Vec4), 1, _File);
	fwrite(&m_Module.vSpawnMinScale, sizeof(Vec4), 1, _File);
	fwrite(&m_Module.vSpawnMaxScale, sizeof(Vec4), 1, _File);

	fwrite(&m_Module.MinLife, sizeof(float), 1, _File);
	fwrite(&m_Module.MaxLife, sizeof(float), 1, _File);

	fwrite(&m_Module.SpawnShape, sizeof(UINT), 1, _File);
	fwrite(&m_Module.SpawnShapeScale, sizeof(Vec3), 1, _File);

	fwrite(&m_Module.BlockSpawnShape, sizeof(UINT), 1, _File);
	fwrite(&m_Module.BlockSpawnShapeScale, sizeof(Vec3), 1, _File);

	fwrite(&m_Module.SpaceType, sizeof(UINT), 1, _File);

	// Spawn Burst
	fwrite(&m_Module.SpawnBurstCount, sizeof(UINT), 1, _File);
	fwrite(&m_Module.SpawnBurstRepeat, sizeof(UINT), 1, _File);
	fwrite(&m_Module.SpawnBurstRepeatTime, sizeof(float), 1, _File);

	// Add Velocity
	fwrite(&m_Module.AddVelocityType, sizeof(UINT), 1, _File);
	fwrite(&m_Module.AddVelocityFixedDir, sizeof(Vec3), 1, _File);
	fwrite(&m_Module.AddMinSpeed, sizeof(float), 1, _File);
	fwrite(&m_Module.AddMaxSpeed, sizeof(float), 1, _File);

	// Scale
	fwrite(&m_Module.StartScale, sizeof(float), 1, _File);
	fwrite(&m_Module.EndScale, sizeof(float), 1, _File);

	// Drag
	fwrite(&m_Module.DestNormalizedAge, sizeof(float), 1, _File);
	fwrite(&m_Module.LimitSpeed, sizeof(float), 1, _File);

	// Noise Force
	fwrite(&m_Module.NoiseForceTerm, sizeof(float), 1, _File);
	fwrite(&m_Module.NoiseForceScale, sizeof(float), 1, _File);

	// Render
	fwrite(&m_Module.EndColor, sizeof(Vec3), 1, _File);
	fwrite(&m_Module.FadeOut, sizeof(UINT), 1, _File);
	fwrite(&m_Module.FadeOutStartRatio, sizeof(float), 1, _File);
	fwrite(&m_Module.VelocityAlignment, sizeof(UINT), 1, _File);

	for (UINT i = 0; i < (UINT)PARTICLE_MODULE::END; ++i)
	{
		fwrite(&m_Module.Module[i], sizeof(int), 1, _File);
	}
}

void CParticleSystem::LoadFromFile(FILE* _File)
{
	// Max Particle Count
	fread(&m_MaxParticleCount, sizeof(int), 1, _File);
	
	// Paticle Texture
	LoadAssetRef(m_ParticleTex, _File);
	
	// Module
	// Spawn
	fread(&m_Module.SpawnRate, sizeof(UINT), 1, _File);
	fread(&m_Module.vSpawnColor, sizeof(Vec4), 1, _File);
	fread(&m_Module.vSpawnMinScale, sizeof(Vec4), 1, _File);
	fread(&m_Module.vSpawnMaxScale, sizeof(Vec4), 1, _File);
	
	fread(&m_Module.MinLife, sizeof(float), 1, _File);
	fread(&m_Module.MaxLife, sizeof(float), 1, _File);
	
	fread(&m_Module.SpawnShape, sizeof(UINT), 1, _File);
	fread(&m_Module.SpawnShapeScale, sizeof(Vec3), 1, _File);
	
	fread(&m_Module.BlockSpawnShape, sizeof(UINT), 1, _File);
	fread(&m_Module.BlockSpawnShapeScale, sizeof(Vec3), 1, _File);
	
	fread(&m_Module.SpaceType, sizeof(UINT), 1, _File);
	
	// Spawn Burst
	fread(&m_Module.SpawnBurstCount, sizeof(UINT), 1, _File);
	fread(&m_Module.SpawnBurstRepeat, sizeof(UINT), 1, _File);
	fread(&m_Module.SpawnBurstRepeatTime, sizeof(float), 1, _File);
	
	// Add Velocity
	fread(&m_Module.AddVelocityType, sizeof(UINT), 1, _File);
	fread(&m_Module.AddVelocityFixedDir, sizeof(Vec3), 1, _File);
	fread(&m_Module.AddMinSpeed, sizeof(float), 1, _File);
	fread(&m_Module.AddMaxSpeed, sizeof(float), 1, _File);
	
	// Scale
	fread(&m_Module.StartScale, sizeof(float), 1, _File);
	fread(&m_Module.EndScale, sizeof(float), 1, _File);
	
	// Drag
	fread(&m_Module.DestNormalizedAge, sizeof(float), 1, _File);
	fread(&m_Module.LimitSpeed, sizeof(float), 1, _File);
	
	// Noise Force
	fread(&m_Module.NoiseForceTerm, sizeof(float), 1, _File);
	fread(&m_Module.NoiseForceScale, sizeof(float), 1, _File);
	
	// Render
	fread(&m_Module.EndColor, sizeof(Vec3), 1, _File);
	fread(&m_Module.FadeOut, sizeof(UINT), 1, _File);
	fread(&m_Module.FadeOutStartRatio, sizeof(float), 1, _File);
	fread(&m_Module.VelocityAlignment, sizeof(UINT), 1, _File);
	
	for (UINT i = 0; i < (UINT)PARTICLE_MODULE::END; ++i)
	{
		fread(&m_Module.Module[i], sizeof(int), 1, _File);
	}
}


