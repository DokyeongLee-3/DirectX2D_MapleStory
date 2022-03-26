#pragma once

#include "GameInfo.h"
#include "Widget/ConfigurationWindow.h"
#include "Widget/Inventory.h"
#include "Widget/SkillQuickSlotWindow.h"
#include "Widget/CharacterEXP.h"
#include "Widget/CharacterStatusWindow.h"
#include "Widget/BossMatching.h"
#include "Widget/StatWindow.h"
#include "Widget/DyingNoticeWindow.h"

struct MonsterInfo
{
	std::string Name;
	int Level;
	int HPMax;
	int HP;
	int Attack;

	MonsterInfo() :
		Level(0),
		HPMax(100),
		HP(100),
		Attack(10)
	{
	}
};

struct ZakumInfo
{
	int Level;
	int BodyHP;
	int BodyHPMax;
	int LeftArm1HP;
	int LeftArm2HP;
	int LeftArm3HP;
	int LeftArm4HP;
	int RightArm1HP;
	int RightArm2HP;
	int RightArm3HP;
	int RightArm4HP;
	int ClapAttack;
	int SmashAttack;
	int FireAttack;

	ZakumInfo() :
		Level(200),
		BodyHP(300000),
		BodyHPMax(300000),
		LeftArm1HP(30000),
		LeftArm2HP(30000),
		LeftArm3HP(30000),
		LeftArm4HP(30000),
		RightArm1HP(30000),
		RightArm2HP(30000),
		RightArm3HP(30000),
		RightArm4HP(30000),
		ClapAttack(9999),
		SmashAttack(100),
		FireAttack(100)
	{
	}
};

class CClientManager
{
public:
	bool Init(HINSTANCE hInst);
	void CreateStartSceneMode();
	int Run();

private:
	CConfigurationWindow*    m_ConfigurationWindow;
	CInventory*              m_Inventory;
	CSkillQuickSlotWindow*   m_SkillQuickSlot;
	CCharacterStatusWindow*  m_CharacterStatusWindow;
	CCharacterEXP*           m_CharacterEXPWindow;
	CBossMatching*           m_BossMatchingWindow;
	CStatWindow*			 m_StatWindow;
	CDyingNoticeWindow*      m_DyingNoticeWindow;

	std::vector<MonsterInfo> m_vecMonsterInfo;
	ZakumInfo					m_ZakumInfo;

public:
	void SetStatWindow(CStatWindow* Window)
	{
		m_StatWindow = Window;
	}

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
	
	void SetDyingNoticeWindow(CDyingNoticeWindow* Window)
	{
		m_DyingNoticeWindow = Window;
	}

public:

	CCharacterEXP* GetEXPWindow()	const
	{
		return m_CharacterEXPWindow;
	}

	CDyingNoticeWindow* GetDyingNoticeWindow()	const
	{
		return m_DyingNoticeWindow;
	}

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

	CStatWindow* GetStatWindow()	const
	{
		return m_StatWindow;
	}

	const ZakumInfo&	GetZakumInfo()	const
	{
		return m_ZakumInfo;
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
	void OnOffBossMatching(float DeltaTime);
	void OnOffStatWindow(float DeltaTime);

public:
	//bool IsCritical(int Factor);

	void NextMonsterState(Monster_State& State);
	MonsterInfo FindMonsterInfo(const std::string& Name);

	DECLARE_SINGLE(CClientManager)
};

