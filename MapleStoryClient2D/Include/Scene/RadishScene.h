#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/ConfigurationWindow.h"
#include "../Widget/Inventory.h"
#include "../Widget/SkillQuickSlotWindow.h"
#include "../Widget/CharacterEXP.h"
#include "../Widget/CharacterStatusWindow.h"

class CRadishScene :
    public CSceneMode
{
public:
    CRadishScene();
    ~CRadishScene();

private:
    class CStage* m_StageObject;

private:
    //CSharedPtr<CMainWidget>             m_MainWidget;
    CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
    CSharedPtr<CInventory>              m_Inventory;
    CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
    CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
    CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;

    std::function<void(bool, float)>    m_LoadingFunction;
    class CLoadingThread* m_LoadingThread;

    std::list<class CRadishMonster*>   m_RadishMonsterList;

public:
    void PushRadishMonster(class CRadishMonster* Monster);

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
    void CreateMonsterAnimationSequence();
    void CreateMapAnimationSequence();
    void CreateEffectPrototype();
    void LoadSound();
    void AddTileCollisionCallback();

public:
    class CRadishMonster* FindRadishMonster(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint);

public:
    void CreateWayToZakumScene();
    void CreateLobbyScene();
    void CreateOnionScene();

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

