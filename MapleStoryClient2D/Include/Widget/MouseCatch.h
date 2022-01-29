#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

class CMouseCatch :
    public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;
	friend class CEngine;

protected:
	CMouseCatch();
	CMouseCatch(const CMouseCatch& window);
	virtual ~CMouseCatch();

private:
	CSharedPtr<CImage>	m_Image;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CMouseCatch* Clone();
};

