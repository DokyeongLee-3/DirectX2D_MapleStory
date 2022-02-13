#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Button.h"

class CBossMatching :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CBossMatching();
	CBossMatching(const CBossMatching& window);
	virtual ~CBossMatching();

private:
	CSharedPtr<CImage> m_Background;
	CSharedPtr<CButton>	m_Button;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CBossMatching* Clone();

public:
	void CreateWayToZakumScene();
};

