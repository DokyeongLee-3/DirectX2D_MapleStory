#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CMouseClick :
    public CWidgetWindow
{

	friend class CViewport;
	friend class CWidgetComponent;
	friend class CEngine;

protected:
	CMouseClick();
	CMouseClick(const CMouseClick& window);
	virtual ~CMouseClick();

private:
	CSharedPtr<CImage>	m_Image;

public:
	CImage* GetCursorImage()	const
	{
		return m_Image;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CMouseClick* Clone();
};

