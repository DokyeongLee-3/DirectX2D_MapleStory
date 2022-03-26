#pragma once

#include "Widget.h"

class CImage :
	public CWidget
{
	friend class CWidgetWindow;

protected:
	CImage();
	CImage(const CImage& widget);
	virtual ~CImage();

protected:
	WidgetImageInfo	m_Info;
	std::function<void()>	m_ClickCallback;
	std::function<void()>	m_DoubleClickCallback;
	std::function<void()>	m_HoverCallback;
	std::function<void()>	m_HoverEndCallback;
	bool m_MoveXAllowed;
	bool m_MoveYAllowed;
	bool m_Clicked;
	int m_ClickCount;
	int m_FrameCount;
	bool m_PrevFrameClick;
	bool m_CurrentFrameClick;
	bool m_PrevHovered;

public:
	void SetClicked(bool Clicked)
	{
		m_Clicked = Clicked;
	}

	void SetMoveXAllowed(bool Allowed)
	{
		m_MoveXAllowed = Allowed;
	}

	void SetMoveYAllowed(bool Allowed)
	{
		m_MoveYAllowed = Allowed;
	}

	virtual void Enable(bool bEnable)
	{
		CWidget::Enable(bEnable);
	}

	void SetTexture(CTexture* Texture)
	{
		m_Info.Texture = Texture;

		SetUseTexture(true);
	}

	const WidgetImageInfo& GetInfo()
	{
		return m_Info;
	}

	void ClearWidgetFrameData()
	{
		m_Info.vecFrameData.clear();
	}

	bool IsClicked()	const
	{
		return m_Clicked;
	}

	bool SetTexture(const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& Name,
		const TCHAR* FullPath);
	bool SetTexture(const std::string& Name, const std::vector<TCHAR*>& vecFileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(const std::string& Name,
		const std::vector<TCHAR*>& vecFullPath);
	void SetTextureTint(const Vector4& Tint);
	void SetTextureTint(unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void AddFrameData(const Vector2& Start, const Vector2& Size);
	void AddFrameData(int Count);
	void SetPlayTime(float PlayTime);
	void SetPlayScale(float PlayScale);
	void LoadSequence2D(const char* FileName, const std::string& PathName = ANIMATION_PATH, float PlayTime = 1.f,
		float PlayScale = 1.f);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CImage* Clone();

public:
	template <typename T>
	void SetDoubleClickCallback(T* Obj, void (T::* Func)())
	{
		m_DoubleClickCallback = std::bind(Func, Obj);
	}

	template <typename T>
	void SetClickCallback(T* Obj, void (T::* Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}

	void SetClickCallback(void (*Func)())
	{
		m_ClickCallback = std::bind(Func);
	}

	template <typename T>
	void SetHoverCallback(T* Obj, void (T::* Func)())
	{
		m_HoverCallback = std::bind(Func, Obj);
	}

	template <typename T>
	void SetHoverEndCallback(T* Obj, void (T::* Func)())
	{
		m_HoverEndCallback = std::bind(Func, Obj);
	}
};

