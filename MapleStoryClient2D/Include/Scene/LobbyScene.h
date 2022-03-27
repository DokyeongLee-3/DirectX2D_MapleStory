#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/ConfigurationWindow.h"
#include "../Widget/Inventory.h"
#include "../Widget/SkillQuickSlotWindow.h"
#include "../Widget/CharacterEXP.h"
#include "../Widget/CharacterStatusWindow.h"
#include "../Widget/BossMatching.h"
#include "../Widget/StatWindow.h"
#include "../Widget/DyingNoticeWindow.h"
#include "../Widget/SkillPointWindow.h"

class CLobbyScene :
    public CSceneMode
{
public:
	CLobbyScene();
	~CLobbyScene();

private:
	class CStage* m_StageObject;

private:
    //CSharedPtr<CMainWidget>             m_MainWidget;

    CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
    CSharedPtr<CInventory>              m_Inventory;
    CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
    CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
    CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;
    CSharedPtr<CBossMatching>           m_BossMatchingWindow;
    CSharedPtr<CStatWindow>             m_StatWindow;
    CSharedPtr<CDyingNoticeWindow>      m_DyingNoticeWindow;
    CSharedPtr<CSkillPointWindow>       m_SkillPointWindow;

    std::function<void(bool, float)>    m_LoadingFunction;
    class CLoadingThread*               m_LoadingThread;

public:
    CCharacterStatusWindow* GetCharacterStatusWindow()   const
    {
        return m_CharacterStatusWindow;
    }

    CCharacterEXP* GetCharacterEXPWindow()   const
    {
        return m_CharacterEXPWindow;
    }

    class CStage* GetStageObject()    const
    {
        return m_StageObject;
    }

    void SetStageObject(class CStage* Stage);

    class CLoadingThread* GetLoadingThread()    const;

public:
    virtual void Start();
	virtual bool Init();
    virtual void Update(float DeltaTime);

private:
    void CreateMaterial();
    void CreateAnimationSequence();
    void CreatePlayerAnimationSequence();
    void CreateSkillAnimationSequence();
    //void CreateMonsterAnimationSequence();
    void CreateMapAnimationSequence();
    void LoadSound();
    void AddTileCollisionCallback();
    void CreateInGameWidgetWindow();

public:
    void CreateOnionScene();
    void CreateWayToZakumScene();
    void CreateLibrary2ndScene();

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

