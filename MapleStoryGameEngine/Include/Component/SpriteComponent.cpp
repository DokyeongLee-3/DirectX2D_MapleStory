
#include "SpriteComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Animation/AnimationSequence2DData.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"

CSpriteComponent::CSpriteComponent() :
	m_Animation(nullptr)
{
	SetTypeID<CSpriteComponent>();
	m_Render = true;
	m_Flip = false;
}

CSpriteComponent::CSpriteComponent(const CSpriteComponent& com) :
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_Animation)
		m_Animation = com.m_Animation->Clone();

	if (com.m_Material)
		m_Material = com.m_Material->Clone();
}

CSpriteComponent::~CSpriteComponent()
{
	SAFE_DELETE(m_Animation);
}

CAnimationSequence2DInstance* CSpriteComponent::GetAnimationInstance() const
{
	return m_Animation;
}

CAnimationSequence2DData* CSpriteComponent::GetCurrentAnimation() const
{
	return m_Animation->GetCurrentAnimation();
}

const std::string& CSpriteComponent::GetCurrentAnimationName() const
{
	return m_Animation->GetCurrentAnimation()->GetName();
}


void CSpriteComponent::ChangeAnimation(const std::string& AnimationName)
{
	m_Animation->ChangeAnimation(AnimationName);
}

void CSpriteComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material->Clone();

	m_Material->SetScene(m_Scene);
}

void CSpriteComponent::SetBaseColor(const Vector4& Color)
{
	m_Material->SetBaseColor(Color);
}

void CSpriteComponent::SetBaseColor(float r, float g, float b, float a)
{
	m_Material->SetBaseColor(r, g, b, a);
}

void CSpriteComponent::SetRenderState(CRenderState* State)
{
	m_Material->SetRenderState(State);
}

void CSpriteComponent::SetRenderState(const std::string& Name)
{
	m_Material->SetRenderState(Name);
}

void CSpriteComponent::SetTransparency(bool Enable)
{
	m_Material->SetTransparency(Enable);
}

void CSpriteComponent::SetOpacity(float Opacity)
{
	m_Material->SetOpacity(Opacity);
}

void CSpriteComponent::AddOpacity(float Opacity)
{
	m_Material->AddOpacity(Opacity);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->AddTexture(Register, ShaderType, Name, Texture);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	m_Material->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CSpriteComponent::AddTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CSpriteComponent::SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	m_Material->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CSpriteComponent::SetTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	m_Material->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CSpriteComponent::Start()
{
	CSceneComponent::Start();

	if (m_Animation)
		m_Animation->Start();
}

bool CSpriteComponent::Init()
{
	m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");

	// MaterialManager에서 만들어준 디폴트 Material로 Set
	// 이 디폴트 Material은 EngineTexture를 텍스쳐를 디폴트로 갖고 있음
	// 단순히 찾아서 Set하는게 아니라 복제해서 그걸 내것으로 Set한것
	SetMaterial(m_Scene->GetResource()->FindMaterial("BaseTexture"));

	SetMeshSize(1.f, 1.f, 0.f);
	SetWorldScale((float)m_Material->GetTextureWidth(),
		(float)m_Material->GetTextureHeight(), 1.f);


	return true;
}

void CSpriteComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_Animation)
		m_Animation->Update(DeltaTime);

	if (!m_Animation || !m_Animation->GetCurrentAnimation() || m_Animation->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount() == 0)
		return;

	int Frame = m_Animation->GetCurrentAnimation()->GetCurrentFrame();

	AnimationFrameData Data = m_Animation->GetCurrentAnimation()->GetFrameData(Frame);

	SetWorldScale(Data.Size.x, Data.Size.y, 0.f);
}

void CSpriteComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CSpriteComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CSpriteComponent::Render()
{
	CSceneComponent::Render();

	if (m_Animation)
	{
		CRenderManager::GetInst()->GetStandard2DCBuffer()->SetAnimation2DEnable(m_Animation->GetAnimationCount() > 0);
		CRenderManager::GetInst()->GetStandard2DCBuffer()->UpdateCBuffer();

		m_Animation->SetShader();
	}

	// 최종 완성된 Shader를 파이프라인에 Set해주고
	// 만들었던 입력 레이아웃을 IA단계에 Set해준다
	m_Material->Render();

	// 위상구조, 정점 버퍼, 인덱스 버퍼를 Set해주고
	// DrawIndexed(or Draw)를 호출해서 렌더링 파이프라인 시작
	m_Mesh->Render();

	m_Material->Reset();
}

void CSpriteComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CSpriteComponent* CSpriteComponent::Clone()
{
	return new CSpriteComponent(*this);
}

void CSpriteComponent::Save(FILE* File)
{
	std::string	MeshName = m_Mesh->GetName();

	int	Length = (int)MeshName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	m_Material->Save(File);

	bool	AnimEnable = false;

	if (m_Animation)
		AnimEnable = true;

	fwrite(&AnimEnable, sizeof(bool), 1, File);

	if (m_Animation)
	{
		size_t	TypeID = m_Animation->GetTypeID();
		fwrite(&TypeID, sizeof(size_t), 1, File);

		m_Animation->Save(File);
	}

	CSceneComponent::Save(File);
}

void CSpriteComponent::Load(FILE* File)
{
	char	MeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);

	m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	m_Material = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();

	m_Material->Load(File);

	bool	AnimEnable = false;

	fread(&AnimEnable, sizeof(bool), 1, File);

	if (AnimEnable)
	{
		size_t	TypeID = 0;
		fread(&TypeID, sizeof(size_t), 1, File);

		CSceneManager::GetInst()->CallCreateAnimInstance(this, TypeID);

		m_Animation->Load(File);
	}
		
	CSceneComponent::Load(File);
}

void CSpriteComponent::Flip()
{
	if (!m_Flip)
		m_Flip = true;

	else
		m_Flip = false;

	m_Animation->Flip();
}

