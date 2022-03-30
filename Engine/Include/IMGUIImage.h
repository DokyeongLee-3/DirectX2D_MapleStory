#pragma once

#include "IMGUIWidget.h"
#include "Resource/Texture/Texture.h"

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
	// ���̴����� BaseColor�� ������ �ǹ�
	ImVec4		m_Tint;
	ImVec4		m_BorderColor;

	std::function<void()> m_HoverCallback;

public:
	Vector2 GetImageSize()	const
	{
		Vector2 Size;

		Size.x = (float)m_Texture->GetWidth();
		Size.y = (float)m_Texture->GetHeight();

		return Size;
	}

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

public:
	template <typename T>
	void SetHoverCallback(T* Obj, void (T::* func)())
	{
		m_HoverCallback = std::bind(func, Obj);
	}
};

