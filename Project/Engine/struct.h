#pragma once

struct Vtx
{
	Vec3   vPos;
	Vec4   vColor;
	Vec2   vUV;
};

struct tDebugShapeInfo
{
	DEBUG_SHAPE     Shape;
	Vec3            vPos;
	Vec3            vScale;
	Vec3            vRot;
	Matrix          matWorld;
	Vec4            vColor;
	float           LifeTime;
	float           Age;
	bool            DepthTest;
};

struct tTimer
{
	float                   Duration;
	float                   CurrentTime;
	std::function<void()>   Callback;
	bool                    Repeat;
};

struct tTask
{
	TASK_TYPE    Type;
	DWORD_PTR	 Param_0;
	DWORD_PTR	 Param_1;
};

struct tLight
{
	Vec4    Color;
	Vec4    Ambient;
};

struct tLightInfo
{
	tLight       light;
	Vec3         WorldPos;
	Vec3         WorldDir;
	float        Radius;
	float        Angle;
	LIGHT_TYPE   Type;
	int          padding[3];

};

struct tPixel
{
	BYTE b;
	BYTE g;
	BYTE r;
	BYTE a;
};

struct tParticle
{
	Vec4   vColor;

	Vec3   vLocalPos;
	Vec3   vWorldPos;
	Vec3   vWorldInitScale;
	Vec3   vWroldCurrentScale;
	Vec3   vWorldRotation;

	Vec3   vForce;                    // Particle�� ������ ��
	Vec3   vVelocity;

	float  NoiseForceAccTime;          // Noise Force �ҿ� �����ϴ� ���� �ð�
	Vec3   NoiseForceDir;              // Noise Force�� ����

	float  Age;
	float  Life;
	float  NormalizedAge;
	int    Active;

	Vec2  Padding;
};

// Particle Module
struct tParticleModule
{
	// Spawn
	UINT	SpawnRate;				// �ʴ� ��ƼŬ �������� (Spawn Per Second)
	Vec4	vSpawnColor;			// ���� ���� ����
	Vec4	vSpawnMinScale;			// ���� �� �ּ� ũ��
	Vec4	vSpawnMaxScale;			// ���� �� �ִ� ũ��

	float	MinLife;				// �ּ� ����
	float	MaxLife;				// �ִ� ����

	UINT	SpawnShape;				// 0 : Box,  1: Sphere
	Vec3	SpawnShapeScale;		// SpawnShapeScale.x == Radius

	UINT	BlockSpawnShape;		// 0 : Box,  1: Sphere
	Vec3	BlockSpawnShapeScale;	// SpawnShapeScale.x == Radius

	UINT	SpaceType;				// 0 : LocalSpace, 1 : WorldSpace

	// Spawn Burst
	UINT	SpawnBurstCount;		// �ѹ��� �߻���Ű�� Particle ��
	UINT	SpawnBurstRepeat;
	float	SpawnBurstRepeatTime;

	// Add Velocity
	UINT	AddVelocityType;		// 0 : Random, 1 : FromCenter, 2 : ToCenter, 4 : Fixed 
	Vec3	AddVelocityFixedDir;
	float	AddMinSpeed;
	float	AddMaxSpeed;

	// Scale Module
	float	StartScale;
	float	EndScale;

	// Drag Module
	float	DestNormalizedAge;
	float	LimitSpeed;

	// Noise Force Module
	float	NoiseForceTerm;		// Noise Force �����Ű�� ��
	float	NoiseForceScale;	// Noise Force ũ��

	// Render Module
	Vec3	EndColor;			// ���� ����
	UINT	FadeOut;			// 0 : Off, 1 : Normalized Age
	float	FadeOutStartRatio;	// FadeOut ȿ���� ���۵Ǵ� Normalized Age ����
	UINT	VelocityAlignment;	// �ӵ� ���� 0 : Off, 1 : On


	// Module On / Off
	int		Module[(UINT)PARTICLE_MODULE::END];
};

// ������� ���� ����ü
struct tTransform
{
	Matrix matWorld;
	Matrix matView;
	Matrix matProj;

	Matrix matWV;
	Matrix matWVP;
};
extern tTransform g_Trans;

struct tMtrlConst
{
	 int     iArr[4];
	 float   fArr[4];
	 Vec2    v2Arr[4];
	 Vec4    v4Arr[4];
	 Matrix  matArr[4];
	 int	 btex[TEX_PARAM::END];
};

struct tSpriteInfo
{
	Vec2 LeftTopUV;
	Vec2 SliceUV;
	Vec2 BackGroundUV;
	Vec2 OffsetUV;

	int UseFlipBook;         // FlipBook�� ��� ����
	int Padding[3];          // 16����Ʈ�� �����ֱ� ���� Padding
};

struct tGlobalData
{
	// �ð� ���� ����
	float g_DT;
	float g_EngineDT;
	float g_Time;
	float g_EngineTime;

	// ����Ÿ���� �ػ� ����
	Vec2  g_Resolution;

	// ���ε� �� ����ȭ���ۿ� ������ �� �� ����ִ���
	int   g_Light2DCount;
	int   g_Light3DCount;
};
extern tGlobalData g_GlobalData;