#pragma once

#include "Widget.h"
#include "WidgetWindow.h"


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
	WidgetImageInfo	m_Info[(int)Button_State::Max];
	std::function<void()>	m_ClickCallback;
	CSharedPtr<class CSound>	m_Sound[(int)Button_Sound_State::Max];
	// 버튼위에 마우스가 hover되면 최초 한번만 Sound재생해주기 위해
	// bool변수로 버튼위에 마우스가 최초로 올라왔을때 true로 만들어주고
	// 버튼위를 마우스가 벗어날때 false로 해준다. 이 bool변수가 true면 
	// Sound재생X
	bool			m_MouseOnSound;
	// 마찬가지로 버튼을 계속해서 누르고 있으면 한번만 Sound재생하기 위해 만든 멤버
	bool			m_ClickSound;
	bool			m_UseAnimation;



public:
	Button_State	GetButtonState()	const
	{
		return m_State;
	}

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

		SetUseTexture(true);
	}

	int GetFrameCount(Button_State State)	const
	{
		return (int)m_Info[(int)State].vecFrameData.size();
	}

public:
	void SetPlayTime(Button_State State, float PlayTime)
	{
		m_Info[(int)State].PlayTime = PlayTime;
	}

	void SetPlayScale(Button_State State, float PlayScale)
	{
		m_Info[(int)State].PlayScale = PlayScale;
	}

	bool SetTexture(Button_State State, const std::string& Name, const TCHAR* FileName,
		const std::string& PathName = TEXTURE_PATH);
	bool SetTextureFullPath(Button_State State, const std::string& Name,
		const TCHAR* FullPath);
	void SetTextureTint(Button_State State, const Vector4& Tint);
	void SetTextureTint(Button_State State, unsigned char r, unsigned char g,
		unsigned char b, unsigned char a);
	void AddFrameData(Button_State State, const Vector2& Start, const Vector2& Size);
	void SetSound(Button_Sound_State State, const std::string& Name);
	void SetSound(Button_Sound_State State, class CSound* Sound);
	void SetSound(Button_Sound_State State, const std::string& ChannelGroupName,
		const std::string& Name, const char* FileName,
		const std::string& PathName = SOUND_PATH);

	void LoadSequence2D(Button_State State, const char* FileName, const std::string& PathName = ANIMATION_PATH, float PlayTime = 1.f, 
		float PlayScale = 1.f);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CButton* Clone();

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

