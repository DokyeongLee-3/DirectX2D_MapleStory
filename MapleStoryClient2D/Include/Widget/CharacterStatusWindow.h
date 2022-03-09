#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Number.h"
#include "Widget/Text.h"
#include "Widget/ProgressBar.h"

class CCharacterStatusWindow :
	public CWidgetWindow
{
	friend class CViewport;

protected:
	CCharacterStatusWindow();
	CCharacterStatusWindow(const CCharacterStatusWindow& window);
	virtual ~CCharacterStatusWindow();

private:
	CSharedPtr<CImage> m_HPMPBackground;
	CSharedPtr<CImage> m_Level;
	CSharedPtr<CNumber> m_LevelNumber;
	CSharedPtr<CText>	m_PlayerName;
	CSharedPtr<CProgressBar> m_HPBar;
	CSharedPtr<CProgressBar> m_MPBar;
	CSharedPtr<CNumber> m_HPMaxNumber;
	CSharedPtr<CNumber> m_MPMaxNumber;
	CSharedPtr<CNumber> m_CurrentHPNumber;
	CSharedPtr<CNumber> m_CurrentMPNumber;
	CSharedPtr<CImage> m_HPDelimiter;
	CSharedPtr<CImage> m_MPDelimiter;


public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CCharacterStatusWindow* Clone();

public:
	void SetLevel(int Level)
	{
		m_LevelNumber->SetNumber(Level);
	}

	void SetHPPercent(float Percent)
	{
		if (Percent < m_HPBar->GetPercent())
			m_HPBar->SetOnDecreasing(true);

		else
			m_HPBar->SetOnIncreasing(true);

		m_HPBar->SetSlowPercent(Percent);
	}

	float GetHPPercent()	const
	{
		return m_HPBar->GetPercent();
	}

	void SetMPPercent(float Percent)
	{
		if (Percent < m_MPBar->GetPercent())
			m_MPBar->SetOnDecreasing(true);

		else
			m_MPBar->SetOnIncreasing(true);

		m_MPBar->SetSlowPercent(Percent);
	}

	float GetMPPercent()	const
	{
		return m_MPBar->GetPercent();
	}

	int GetCurrentHP()	const
	{
		return m_CurrentHPNumber->GetNumber();
	}

	int GetCurrentMP()	const
	{
		return m_CurrentMPNumber->GetNumber();
	}

	void SetCurrentHP(int HP)
	{
		m_CurrentHPNumber->SetNumber(HP);
	}

	void SetCurrentMP(int MP)
	{
		m_CurrentMPNumber->SetNumber(MP);
	}
};

