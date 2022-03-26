#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Button.h"

class CDyingNoticeWindow :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CDyingNoticeWindow();
	CDyingNoticeWindow(const CDyingNoticeWindow& window);
	virtual ~CDyingNoticeWindow();

private:
	CSharedPtr<CImage>	m_Background;
	CSharedPtr<CButton>	m_ConfirmButton;
	CSharedPtr<CImage>	m_BlankCollider;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CDyingNoticeWindow* Clone();

public:
	void ConfirmButtonCallback();
	void DragWindow();
};

