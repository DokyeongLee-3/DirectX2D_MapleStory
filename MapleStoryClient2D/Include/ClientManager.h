#pragma once

#include "GameInfo.h"
#include "Widget/ConfigurationWindow.h"
#include "Widget/Inventory.h"
#include "Widget/SkillQuickSlotWindow.h"
#include "Widget/CharacterEXP.h"
#include "Widget/CharacterStatusWindow.h"

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

private:
	CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
	CSharedPtr<CInventory>              m_Inventory;
	CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
	CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
	CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;

public:
	void SetCharacterStatusWindow(CCharacterStatusWindow* Window)
	{
		m_CharacterStatusWindow = Window;
	}

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);

	// 최상위 UI Window 꺼주기
	void TurnOffWindow(float DeltaTime);

	void OnOffInventory(float DeltaTime);
	void OnOffConfiguration(float DeltaTime);

public:
	//bool IsCritical(int Factor);

	DECLARE_SINGLE(CClientManager)
};

