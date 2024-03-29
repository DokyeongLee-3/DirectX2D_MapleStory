#include "IMGUIImage.h"
#include "Resource/ResourceManager.h"

CIMGUIImage::CIMGUIImage() :
	m_ImageStart{},
	m_ImageEnd{},
	m_Tint{ 1.f, 1.f, 1.f, 1.f },
	m_BorderColor{},
	m_HoverCallback(nullptr),
	m_HoverStart(false)
{
}

CIMGUIImage::~CIMGUIImage()
{
}

void CIMGUIImage::SetTexture(const std::string& Name, const TCHAR* FileName, const std::string& PathName)
{
	CResourceManager::GetInst()->LoadTexture(Name, FileName, PathName);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(const std::string& Name)
{
	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTextureFullPath(const std::string& Name, const TCHAR* FullPath)
{
	CResourceManager::GetInst()->LoadTextureFullPath(Name, FullPath);

	m_Texture = CResourceManager::GetInst()->FindTexture(Name);

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

void CIMGUIImage::SetTexture(CTexture* Texture)
{
	m_Texture = Texture;

	SetImageEnd((float)m_Texture->GetWidth(), (float)m_Texture->GetHeight());
}

bool CIMGUIImage::Init()
{
	SetTexture("DefaultUI", TEXT("NoImage.png"));

	SetImageStart(0.f, 0.f);
	SetImageEnd((float)m_Texture->GetWidth() , (float)m_Texture->GetHeight());

	return true;
}

void CIMGUIImage::Render()
{
	if (m_Texture)
	{
		unsigned int	Width = m_Texture->GetWidth();
		unsigned int	Height = m_Texture->GetHeight();

		ImVec2	StartUV = ImVec2(m_ImageStart.x / Width, m_ImageStart.y / Height);
		ImVec2	EndUV = ImVec2(m_ImageEnd.x / Width, m_ImageEnd.y / Height);

		ImGui::Image(m_Texture->GetResource(), m_Size, StartUV, EndUV, m_Tint, m_BorderColor);

		if (ImGui::IsItemHovered() && m_HoverCallback)
		{
			m_HoverStart = true;
			m_HoverCallback();
		}

		// 지난 프레임까지 Hover됐다가 이번 프레임에 Hover되지 않음
		else if (m_HoverEndCallback && !ImGui::IsItemHovered() && m_HoverStart)
		{
			m_HoverEndCallback();
			m_HoverStart = false;
		}

		//m_HoverStart = false;
	}
}
