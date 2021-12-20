
#include "Material.h"
#include "../ResourceManager.h"
#include "../../Scene/Scene.h"
#include "../../Scene/SceneResource.h"
#include "../Shader/MaterialConstantBuffer.h"
#include "../../Render/RenderManager.h"
#include "../../Render/RenderState.h"

CMaterial::CMaterial() :
	m_BaseColor(Vector4::White),
	m_Scene(nullptr),
	m_CBuffer(nullptr),
	m_Opacity(1.f),
	m_RenderStateArray{}
{
	SetTypeID<CMaterial>();
}

CMaterial::CMaterial(const CMaterial& Material)
{
	*this = Material;

	m_RefCount = 0;

	m_Scene = nullptr;
}

CMaterial::~CMaterial()
{
}

void CMaterial::SetRenderState(CRenderState* State)
{
	m_RenderStateArray[(int)RenderState_Type::Blend] = State;
}

void CMaterial::SetRenderState(const std::string& Name)
{
	m_RenderStateArray[(int)RenderState_Type::Blend] = CRenderManager::GetInst()->FindRenderState(Name);
}

void CMaterial::SetTransparency(bool Enable)
{
	if (Enable)
		m_RenderStateArray[(int)RenderState_Type::Blend] = CRenderManager::GetInst()->FindRenderState("AlphaBlend");
}

void CMaterial::SetOpacity(float Opacity)
{
	m_Opacity = Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::AddOpacity(float Opacity)
{
	m_Opacity += Opacity;

	if (m_Opacity < 0.f)
		m_Opacity = 0.f;

	else if (m_Opacity > 1.f)
		m_Opacity = 1.f;
}

void CMaterial::SetBaseColor(const Vector4& Color)
{
	m_BaseColor = Color;
}

void CMaterial::SetBaseColor(float r, float g, float b, float a)
{
	m_BaseColor = Vector4(r, g, b, a);
}

void CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	// Material이 특정 Scene에 소속되어 있다면 SceneResource를 통해서 Texture를 찾거나 Load할 수 있음
	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	m_TextureInfo.push_back(MaterialTextureInfo());

	int	Index = (int)m_TextureInfo.size() - 1;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
}

void CMaterial::AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CTexture* Texture = nullptr;

	// Material이 특정 Scene에 소속되어 있다면 SceneResource를 통해서 Texture를 찾거나 Load할 수 있음
	if (!m_Scene)
	{
		if (!CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = CResourceManager::GetInst()->FindTexture(Name);
	}

	else
	{
		if (!m_Scene->GetResource()->LoadTexture(Name, FileName, PathName))
			return;

		Texture = m_Scene->GetResource()->FindTexture(Name);
	}

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = Texture;
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTextureFullPath(int Index, int Register,
	int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	if (!CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath))
		return;

	m_TextureInfo[Index].Register = Register;
	m_TextureInfo[Index].Name = Name;
	m_TextureInfo[Index].Texture = CResourceManager::GetInst()->FindTexture(Name);
	m_TextureInfo[Index].ShaderType = ShaderType;
}

void CMaterial::SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
}

void CMaterial::SetShader(const std::string& Name)
{
	m_Shader = (CGraphicShader*)CResourceManager::GetInst()->FindShader(Name);
}

void CMaterial::Render()
{
	// 말 그대로 완성된 m_VS, m_Ps등을 SetShader해주는 함수
	if (m_Shader)
		m_Shader->SetShader();

	// 출력 직전에 m_BaseColor 갱신 후
	// 상수버퍼에 갱신한 m_BaseColor로 업데이트
	if (m_CBuffer)
	{
		m_CBuffer->SetBaseColor(m_BaseColor);
		m_CBuffer->SetOpacity(m_Opacity);

		m_CBuffer->UpdateCBuffer();
	}

	for (int i = 0; i < (int)RenderState_Type::Max; ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->SetState();
	}

	size_t	Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		// Texture에 대한 Resource view를 Set해주는 함수
		m_TextureInfo[i].Texture->SetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}
}

void CMaterial::Reset()
{
	size_t	Size = m_TextureInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_TextureInfo[i].Texture->ResetShader(m_TextureInfo[i].Register, m_TextureInfo[i].ShaderType, 0);
	}

	for (int i = 0; i < (int)RenderState_Type::Max; ++i)
	{
		if (m_RenderStateArray[i])
			m_RenderStateArray[i]->ResetState();
	}
}

CMaterial* CMaterial::Clone()
{
	return new CMaterial(*this);
}
