#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Text.h"
#include "Inventory.h"

class CToolTip :
    public CWidgetWindow
{
	friend class CViewport;
	friend class CWidgetComponent;

protected:
	CToolTip();
	CToolTip(const CToolTip& window);
	virtual ~CToolTip();

protected:
	CSharedPtr<CImage>	m_ToolTipBack;
	CSharedPtr<CText>	m_ToolTipTitle;
	CSharedPtr<CText>	m_ToolTipContents;
	CSharedPtr<CImage>	m_ToolTipItemIconBack;
	CSharedPtr<CImage>	m_ToolTipItemIcon;

	CInventory* m_OwnerInventory;


public:
	CImage* GetToolTipBack()	const
	{
		return m_ToolTipBack;
	}

	CText* GetToolTipTitle()	const
	{
		return m_ToolTipTitle;
	}

	CText* GetToolTipContents()	const
	{
		return m_ToolTipContents;
	}

	CImage* GetToolTipItemIconBack()	const
	{
		return m_ToolTipItemIconBack;
	}

	CImage* GetToolTipItemIcon()	const
	{
		return m_ToolTipItemIcon;
	}

	CInventory* GetOwnerInventory()	const
	{
		return m_OwnerInventory;
	}

	void SetOwnerInventory(CInventory* Inven)
	{
		m_OwnerInventory = Inven;
	}

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CToolTip* Clone();
};

