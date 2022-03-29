#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Number.h"
#include "Widget/Text.h"

class CSkillPointWindow :
    public CWidgetWindow
{
	friend class CViewport;

protected:
	CSkillPointWindow();
	CSkillPointWindow(const CSkillPointWindow& window);
	virtual ~CSkillPointWindow();

private:
	CSharedPtr<CImage> m_SkillPointWindowBack;

	CSharedPtr<CImage>	m_ZeroGradeSkillTab;
	CSharedPtr<CImage>	m_FirstGradeSkillTab;
	CSharedPtr<CImage>	m_SecondGradeSkillTab;
	CSharedPtr<CImage>	m_ThirdGradeSkillTab;

	CSharedPtr<CNumber>	m_EnableSkillPoint;
	CSharedPtr<CText>	m_Title;

	CSharedPtr<CImage>	m_FirstGradeSkillBookImage;
	CSharedPtr<CImage>	m_SecondGradeSkillBookImage;
	CSharedPtr<CImage>	m_ThirdGradeSkillBookImage;
	
	std::vector<SkillPointInfo*>	m_vecFirstGradeSkillPointInfo;
	std::vector<SkillPointInfo*>	m_vecSecondGradeSkillPointInfo;
	std::vector<SkillPointInfo*>	m_vecThirdGradeSkillPointInfo;

	int m_CurrentOpenTabNum;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CSkillPointWindow* Clone();

public:
	void ClickFirstGradeSkillTab();
	void ClickSecondGradeSkillTab();
	void ClickThirdGradeSkillTab();
	void AddEnableSkillPoint(int Num);
	void LevelUpDeathSideSkill();
	// 레벨업해서 스킬포인트가 있고, 마스터 안한 스킬 한정으로 스킬레벨 올릴 수 있는 모든 스킬들의 버튼 Enable로 활성화 시켜주기
	void ActivateAllLevelUpButton();
};

