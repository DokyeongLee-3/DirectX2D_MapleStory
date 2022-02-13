#pragma once

#include "GameInfo.h"
#include "Widget/ConfigurationWindow.h"
#include "Widget/Inventory.h"
#include "Widget/SkillQuickSlotWindow.h"
#include "Widget/CharacterEXP.h"
#include "Widget/CharacterStatusWindow.h"
#include "Widget/BossMatching.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateStartSceneMode();
	int Run();

private:
	CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
	CSharedPtr<CInventory>              m_Inventory;
	CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
	CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
	CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;
	CSharedPtr<CBossMatching>           m_BossMatchingWindow;

public:
	void SetCharacterStatusWindow(CCharacterStatusWindow* Window)
	{
		m_CharacterStatusWindow = Window;
	}

	void SetInventoryWindow(CInventory* Window)
	{
		m_Inventory = Window;
	}

	void SetSkillQuickSlot(CSkillQuickSlotWindow* Window)
	{
		m_SkillQuickSlot = Window;
	}

	void SetConfigurationWindow(CConfigurationWindow* Window)
	{
		m_ConfigurationWindow = Window;
	}

	void SetEXPWindow(CCharacterEXP* Window)
	{
		m_CharacterEXPWindow = Window;
	}

	void SetBossMatchingWindow(CBossMatching* Window)
	{
		m_BossMatchingWindow = Window;
	}

public:
	CCharacterStatusWindow* GetCharacterStatusWindow()	const
	{
		return m_CharacterStatusWindow;
	}

	CInventory* GetInventoryWindow()	const
	{
		return m_Inventory;
	}

	CSkillQuickSlotWindow* GetSkillQuickSlotWindow()	const
	{
		return m_SkillQuickSlot;
	}

	CConfigurationWindow* GetConfigurationWindow()	const
	{
		return m_ConfigurationWindow;
	}

	CCharacterEXP* GetCharacterEXPWindow()	const
	{
		return m_CharacterEXPWindow;
	}

	CBossMatching* GetBossMatchingWindow()	const
	{
		return m_BossMatchingWindow;
	}

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);

	// �ֻ��� UI Window ���ֱ�
	void TurnOffWindow(float DeltaTime);

	void OnOffInventory(float DeltaTime);
	void OnOffConfiguration(float DeltaTime);
	void OnOffBossMatching(float DeltaTime);

public:
	//bool IsCritical(int Factor);

	DECLARE_SINGLE(CClientManager)
};

