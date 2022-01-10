#pragma once

#include "Widget.h"
#include "../Resource/Texture/Texture.h"

struct ButtonStateInfo
{
	CSharedPtr<CTexture>	Texture;
	Vector4			Tint;
	std::vector<AnimationFrameData> vecFrameData;

	ButtonStateInfo()
	{
		Tint = Vector4::White;
	}
};

class CButton :
    public CWidget
{
	friend class CWidgetWindow;

protected:
	CButton();
	CButton(const CButton& widget);
	virtual ~CButton();

protected:
	Button_State	m_State;
	ButtonStateInfo	m_Info[(int)Button_State::Max];
	std::function<void()>	m_ClickCallback;

public:
	virtual void Enable(bool bEnable)
	{
		CWidget::Enable(bEnable);
	}

	void ButtonEnable(bool bEnable)
	{
		m_State = bEnable ? Button_State::Normal : Button_State::Disable;
	}

	void SetTexture(Button_State State, CTexture* Texture)
	{
		m_Info[(int)State].Texture = Texture;
	}

	bool SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(Button_State State, const std::string& Name,
		const TCHAR* FullPath);
	void SetTextureTint(Button_State State, const Vector4& Tint);
	void SetTextureTint(Button_State State, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();

public:
	template <typename T>
	void SetClickCallback(T* Obj, void (T::* Func)())
	{
		m_ClickCallback = std::bind(Func, Obj);
	}

	void SetClickCallback(void (*Func)())
	{
		m_ClickCallback = std::bind(Func);
	}
};

