#pragma once

#include "IMGUIWidget.h"

class CIMGUIImage :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIImage();
	virtual ~CIMGUIImage();

protected:
	CSharedPtr<class CTexture>	m_Texture;
	ImVec2		m_ImageStart;
	ImVec2		m_ImageEnd;
	// 셰이더에서 BaseColor랑 동일한 의미
	ImVec4		m_Tint;
	ImVec4		m_BorderColor;

public:
	void SetImageStart(float x, float y)
	{
		m_ImageStart = ImVec2(x, y);
	}

	void SetImageEnd(float x, float y)
	{
		m_ImageEnd = ImVec2(x, y);
	}

	Vector2 GetImageStart() const
	{
		Vector2 ImageStart = Vector2(m_ImageStart.x, m_ImageStart.y);

		return ImageStart;
	}

	Vector2 GetImageEnd() const
	{
		Vector2 ImageEnd = Vector2(m_ImageEnd.x, m_ImageEnd.y);

		return ImageEnd;
	}


	void SetTint(unsigned char r, unsigned char g, unsigned char b)
	{
		m_Tint = ImVec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}

	void SetBorderColor(unsigned char r, unsigned char g, unsigned char b)
	{
		m_BorderColor = ImVec4(r / 255.f, g / 255.f, b / 255.f, 1.f);
	}

public:
	void SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	void SetTexture(const std::string& Name);
	void SetTextureFullPath(const std::string& Name, const TCHAR* FullPath);
	void SetTexture(class CTexture* Texture);

public:
	virtual bool Init();
	virtual void Render();
};

