#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Button.h"
#include "Widget/Image.h"

class CConfigurationWindow :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CConfigurationWindow();
	CConfigurationWindow(const CConfigurationWindow& window);
	virtual ~CConfigurationWindow();

private:
	CSharedPtr<CImage> m_Configuration;
	CSharedPtr<CImage>	m_BGMSoundSlider;

	float m_BGMSoundSliderLeftMostXPos;
	float m_BGMSoundSliderRightMostXPos;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CConfigurationWindow* Clone();

public:
	void MoveSlider(float DeltaTime);
};

