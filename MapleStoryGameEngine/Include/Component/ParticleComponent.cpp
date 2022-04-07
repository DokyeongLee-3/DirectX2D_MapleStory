
#include "ParticleComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "../Render/RenderManager.h"

CParticleComponent::CParticleComponent()	:
	m_SpawnTime(0.f),
	m_SpawnTimeMax(0.01f),
	m_Info{},
	m_InfoShared{},
	m_CBuffer(nullptr)
{
	SetTypeID<CParticleComponent>();
	m_Render = true;

	m_LayerName = "Particle";

	float ZValue = CRenderManager::GetInst()->GetLayerUpperBoundZOrder("Particle");

	Vector3 Pos = GetWorldPos();

	SetWorldPos(Pos.x, Pos.y, ZValue);
}

CParticleComponent::CParticleComponent(const CParticleComponent& com) :
	CSceneComponent(com)
{
	m_SpawnTime = 0.f;
	m_SpawnTimeMax = com.m_SpawnTimeMax;

	m_Particle = com.m_Particle;

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	m_vecStructuredBuffer.clear();

	if (m_Particle)
	{
		m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

		m_UpdateShader = m_Particle->GetUpdateShader();

		SAFE_DELETE(m_CBuffer);

		m_CBuffer = m_Particle->CloneConstantBuffer();
	}
}

CParticleComponent::~CParticleComponent()
{
	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);
}

void CParticleComponent::SetParticle(const std::string& Name)
{
	SetParticle(m_Scene->GetResource()->FindParticle(Name));
}

void CParticleComponent::SetParticle(CParticle* Particle)
{
	m_Particle = Particle;

	m_Material = m_Particle->CloneMaterial();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		SAFE_DELETE(m_vecStructuredBuffer[i]);
	}

	SAFE_DELETE(m_CBuffer);

	m_vecStructuredBuffer.clear();

	m_Particle->CloneStructuredBuffer(m_vecStructuredBuffer);

	m_UpdateShader = m_Particle->CloneUpdateShader();

	m_CBuffer = m_Particle->CloneConstantBuffer();

	m_SpawnTimeMax = m_Particle->GetSpawnTime();
}

void CParticleComponent::SetSpawnTime(float Time)
{
	m_SpawnTimeMax = Time;

	m_Particle->SetSpawnTime(m_SpawnTimeMax);
}

void CParticleComponent::SetSpawnTimeMax(float Time)
{
	m_SpawnTimeMax = Time;
}

void CParticleComponent::Start()
{
	CSceneComponent::Start();
}

bool CParticleComponent::Init()
{
	m_Mesh = m_Scene->GetResource()->FindMesh("ParticlePointMesh");

	SetLayerName("Particle");

	return true;
}

void CParticleComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	m_SpawnTime += DeltaTime;

	if (m_SpawnTime >= m_SpawnTimeMax)
	{
		m_SpawnTime -= m_SpawnTimeMax;
		m_CBuffer->SetSpawnEnable(1);
	}

	// Spawn�� Ÿ�̹��� �ƴϸ� Position������ Update
	else
	{
		m_CBuffer->SetSpawnEnable(0);
	}
}

void CParticleComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	CParticleConstantBuffer* CBuffer = m_Particle->GetCBuffer();

	// Update Shader�� ���۽�Ų��.
	Vector3	StartMin, StartMax;

	Vector3 Current = GetWorldPos();

	StartMin = GetWorldPos() + CBuffer->GetStartMin();
	StartMax = GetWorldPos() + CBuffer->GetStartMax();

	m_CBuffer->SetStartMin(StartMin);
	m_CBuffer->SetStartMax(StartMax);

	m_CBuffer->UpdateCBuffer();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader();
	}

	// UpdateShader�� Thread�� 64, 1, 1�� ����ϰ� �ִ�.
	// �����Ǿ��� ��ƼŬ�� ��ü ������ 64���� ������. ���� 64���� �ִ���ƼŬ ���� �������־��ٸ�
	// �ʿ��� �׷��� ���� 1���̴�. ������ 64�� �̸��̶�� 64�� ���� ��� 0�� �����Ƿ� ���⿡ 1�� �����־�� �Ѵ�.
	// 100���� ��� �׷��� 2���� �����ȴ�. �̶� ������� 128���� �ǹǷ� 100���� ������ ������ 28���� ó���� �ȵǰ�
	// �����ָ� �Ǵ°��̴�.
	// ��, UpdateShader�� [numthread(64,1,1)]�� ���� 64���� ������ '�׷�'�� ������ ���̰�, ���⼭ Excute(x,y,z)�� ��������
	// Thread�� x*y*z ���� ���� �� ���̴�(�翬�� UpdateShader���� ThreadID�� 0 ~ x*y*z - 1���� �־��� ��)
	// �ᱹ x*y*z���� �����尡 64���� �׷쳻�� �ڵ������� �й�� ���̴�.
	int	GroupCount = m_Particle->GetSpawnCountMax();
	m_UpdateShader->Excute(GroupCount, 1, 1);


	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader();
	}
}

void CParticleComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CParticleComponent::Render()
{
	CSceneComponent::Render();

	size_t	BufferCount = m_vecStructuredBuffer.size();

	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->SetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}

	if (m_Material)
		m_Material->Render();



	// �ν��Ͻ��� �̿��ؼ� �׷��ش�.
	m_Mesh->RenderInstancing(m_CBuffer->GetSpawnCount());


	for (size_t i = 0; i < BufferCount; ++i)
	{
		m_vecStructuredBuffer[i]->ResetShader(30 + (int)i, (int)Buffer_Shader_Type::Geometry);
	}


	if (m_Material)
		m_Material->Reset();
}

void CParticleComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CParticleComponent* CParticleComponent::Clone()
{
	return new CParticleComponent(*this);
}

void CParticleComponent::Save(FILE* File)
{
	std::string	ParticleName = m_Particle->GetName();

	int	Length = (int)ParticleName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(ParticleName.c_str(), sizeof(char), Length, File);

	fwrite(&m_SpawnTimeMax, sizeof(float), 1, File);

	std::string	MeshName = m_Mesh->GetName();

	Length = (int)MeshName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	m_Material->Save(File);

	CSceneComponent::Save(File);
}

void CParticleComponent::Load(FILE* File)
{
	char	ParticleName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(ParticleName, sizeof(char), Length, File);

	SetParticle(ParticleName);

	fread(&m_SpawnTimeMax, sizeof(float), 1, File);

	char	MeshName[256] = {};

	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);

	m_Mesh = m_Scene->GetResource()->FindMesh(MeshName);

	m_Material = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();

	m_Material->Load(File);

	CSceneComponent::Load(File);
}
