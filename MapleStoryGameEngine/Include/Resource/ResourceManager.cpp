
#include "ResourceManager.h"

DEFINITION_SINGLE(CResourceManager)

CResourceManager::CResourceManager() :
	m_MeshManager(nullptr),
	m_ShaderManager(nullptr),
	m_MaterialManager(nullptr),
	m_TextureManager(nullptr),
	m_AnimationManager(nullptr),
	m_SoundManager(nullptr)
{
}

CResourceManager::~CResourceManager()
{
	SAFE_DELETE(m_SoundManager);
	SAFE_DELETE(m_AnimationManager);
	SAFE_DELETE(m_MaterialManager);
	SAFE_DELETE(m_ShaderManager);
	SAFE_DELETE(m_MeshManager);
	SAFE_DELETE(m_TextureManager);
}

bool CResourceManager::Init()
{
	m_MeshManager = new CMeshManager;

	if (!m_MeshManager->Init())
		return false;

	m_ShaderManager = new CShaderManager;

	// fx���Ϸε� ���̴��� �ε��ؼ� �������ؼ� ID3D11VertexShader* m_VS���� ���̴� �������̽��� �����
	// �Է� ���̾ƿ��� ���� ���̴��� Set���ְ�, �������� ������۵��� �����.
	// ���⼭�� ������۵��� Ÿ���� �������� �ʰ� �׳� CConstantBufferŸ�Ե���
	// ShaderManager�� map�� �־����, �Ʒ��� �� Manager��(ex. MaterialManager,
	// AnimationManager)�� Init���� MatarialConstantBufferŸ������ Ŭ������
	// ���� �����ؼ�, ShaderManager���� ���� CConstantBuffer�� ã�Ƽ� Set���� 
	if (!m_ShaderManager->Init())
		return false;

	m_TextureManager = new CTextureManager;

	// 3������ Sampler�� �����, Sampler���� �� ���̴��� Set���ش�
	// ���÷��� Ư�� Ŭ������ ��� �ִ°� �ƴ϶� ShaderInfo.fx�� ����������
	// ����Ǿ� �־ ���̴� ��𼭵� ���� �����ϴ�
	if (!m_TextureManager->Init())
		return false;

	m_MaterialManager = new CMaterialManager;

	// ��� Material�� ���⼭ ���������, ������� Material�� ����� �����ִ�
	// ������� m_CBuffer�� CMaterialManager::CreateMaterial���� Set����
	// Material�� Texture, Shader, CMaterialConstantBuffer, BaseColor���� ����� ���� ����
	if (!m_MaterialManager->Init())
		return false;

	m_AnimationManager = new CAnimationManager;

	// ShaderManager���� ���� CConstantBuffer�� ã�Ƽ� 
	// m_Animation2DCBuffer�� ��� m_Buffer�� Set���� 
	if (!m_AnimationManager->Init())
		return false;

	m_SoundManager = new CSoundManager;

	if (!m_SoundManager->Init())
		return false;

	return true;
}

void CResourceManager::Update()
{
	m_SoundManager->Update();
}


CMesh* CResourceManager::FindMesh(const std::string& Name)
{
	return m_MeshManager->FindMesh(Name);
}

void CResourceManager::ReleaseMesh(const std::string& Name)
{
	m_MeshManager->ReleaseMesh(Name);
}

CShader* CResourceManager::FindShader(const std::string& Name)
{
	return m_ShaderManager->FindShader(Name);
}

void CResourceManager::ReleaseShader(const std::string& Name)
{
	m_ShaderManager->ReleaseShader(Name);
}

bool CResourceManager::CreateConstantBuffer(const std::string& Name,
	int Size, int Register, int ConstantBufferShaderType)
{
	return m_ShaderManager->CreateConstantBuffer(Name, Size, Register, ConstantBufferShaderType);
}

CConstantBuffer* CResourceManager::FindConstantBuffer(const std::string& Name)
{
	return m_ShaderManager->FindConstantBuffer(Name);
}

CMaterial* CResourceManager::FindMaterial(const std::string& Name)
{
	return m_MaterialManager->FindMaterial(Name);
}

void CResourceManager::ReleaseMaterial(const std::string& Name)
{
	m_MaterialManager->ReleaseMaterial(Name);
}

bool CResourceManager::LoadTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	return m_TextureManager->LoadTexture(Name, FileName, PathName);
}

bool CResourceManager::LoadTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	return m_TextureManager->LoadTextureFullPath(Name, FullPath);
}

CTexture* CResourceManager::FindTexture(const std::string& Name)
{
	return m_TextureManager->FindTexture(Name);
}

void CResourceManager::ReleaseTexture(const std::string& Name)
{
	m_TextureManager->ReleaseTexture(Name);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& Name, const std::string& TextureName,
	const TCHAR* FileName, const std::string& PathName)
{
	return m_AnimationManager->CreateAnimationSequence2D(Name, TextureName, FileName, PathName);
}

bool CResourceManager::CreateAnimationSequence2D(const std::string& Name, CTexture* Texture)
{
	return m_AnimationManager->CreateAnimationSequence2D(Name, Texture);
}

void CResourceManager::EraseAnimationSequence2D(const std::string& Name)
{
	if (FindAnimationSequence2D(Name))
	{
		m_AnimationManager->EraseAnimationSequence2D(Name);
	}
}

void CResourceManager::AddAnimationSequence2DFrame(const std::string& Name, const Vector2& Start,
	const Vector2& Size)
{
	m_AnimationManager->AddFrame(Name, Start, Size);
}

void CResourceManager::AddAnimationSequence2DFrame(const std::string& Name, float StartX,
	float StartY, float Width, float Height)
{
	m_AnimationManager->AddFrame(Name, StartX, StartY, Width, Height);
}

bool CResourceManager::SaveSequence2DFullPath(const std::string& Name, const char* FullPath)
{
	return m_AnimationManager->SaveSequenceFullPath(Name, FullPath);
}

bool CResourceManager::LoadSequence2DFullPath(std::string& resultName, const char* FullPath, class CScene* Scene)
{
	return m_AnimationManager->LoadSequenceFullPath(resultName, FullPath, Scene);
}

bool CResourceManager::LoadSequence2DFullPath(const char* FullPath, CScene* Scene)
{
	std::string	Name;
	return m_AnimationManager->LoadSequenceFullPath(Name, FullPath, Scene);
}

bool CResourceManager::SaveSequence2D(const std::string& Name, const char* FileName, const std::string& PathName)
{
	return m_AnimationManager->SaveSequence(Name, FileName, PathName);
}

bool CResourceManager::LoadSequence2D(const char* FileName, const std::string& PathName, CScene* Scene)
{
	std::string	Name;
	return m_AnimationManager->LoadSequence(Name, FileName, PathName, Scene);
}

bool CResourceManager::LoadSequence2D(std::string& resultName, const char* FileName, const std::string& PathName, CScene* Scene)
{
	return m_AnimationManager->LoadSequence(resultName, FileName, PathName, Scene);
}

CAnimationSequence2D* CResourceManager::FindAnimationSequence2D(const std::string& Name)
{
	return m_AnimationManager->FindSequence(Name);
}

void CResourceManager::ReleaseAnimationSequence2D(const std::string& Name)
{
	m_AnimationManager->ReleaseSequence(Name);
}

CAnimation2DConstantBuffer* CResourceManager::GetAnimation2DCBuffer() const
{
	return m_AnimationManager->GetAnimation2DCBuffer();
}

void CResourceManager::InsertAnimationSequence2D(CAnimationSequence2D* Sequence)
{
	m_AnimationManager->InsertAnimationSequence2D(Sequence);
}


