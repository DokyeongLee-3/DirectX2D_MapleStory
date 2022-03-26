#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Number.h"
#include "Widget/Text.h"
#include "Widget/Image.h"
#include "Widget/Button.h"

class CStatWindow :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CStatWindow();
	CStatWindow(const CStatWindow& window);
	virtual ~CStatWindow();

private:
	CSharedPtr<CImage>	m_Background;
	CSharedPtr<CImage>	m_Name;
	CSharedPtr<CImage>	m_Job;
	CSharedPtr<CNumber>	m_HP;
	CSharedPtr<CNumber>	m_MP;
	CSharedPtr<CNumber>	m_HPMax;
	CSharedPtr<CNumber>	m_MPMax;
	CSharedPtr<CImage>	m_HPDelimiter;
	CSharedPtr<CImage>	m_MPDelimiter;
	CSharedPtr<CNumber>	m_STR;
	CSharedPtr<CNumber>	m_DEX;
	CSharedPtr<CNumber>	m_INT;
	CSharedPtr<CNumber>	m_LUK;
	CSharedPtr<CNumber>	m_AbilityPoint;
	CSharedPtr<CButton>	m_AutoAllocate;
	CSharedPtr<CButton> m_STRUp;
	CSharedPtr<CButton> m_DEXUp;
	CSharedPtr<CButton> m_INTUp;
	CSharedPtr<CButton> m_LUKUp;

	CSharedPtr<CImage>	m_BlankCollider;

public:
	void SetHP(int Number);
	void SetMP(int Number);
	void SetHPMax(int Number);
	void SetMPMax(int Number);
	void SetAbilityPoint(int Number);
	void AddAbilityPoint(int Number);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CStatWindow* Clone();

public:
	void DragWindow();
	void AutoAllocateButton();
	void UpSTRButton();
	void UpDEXButton();
	void UpINTButton();
	void UpLUKButton();
};

