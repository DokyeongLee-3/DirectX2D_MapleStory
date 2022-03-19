#pragma once

#include "Scene/SceneMode.h"
//#include "../Widget/ConfigurationWindow.h"
//#include "../Widget/Inventory.h"
//#include "../Widget/SkillQuickSlotWindow.h"
//#include "../Widget/CharacterEXP.h"
//#include "../Widget/CharacterStatusWindow.h"
//#include "../Widget/StatWindow.h"

class CWayToZakumScene :
    public CSceneMode
{
public:
    CWayToZakumScene();
    ~CWayToZakumScene();

private:
    class CStage* m_StageObject;

private:
    //CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
    //CSharedPtr<CInventory>              m_Inventory;
    //CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
    //CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
    //CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;
    //CSharedPtr<CStatWindow>             m_StatWindow;

    std::function<void(bool, float)>    m_LoadingFunction;
    class CLoadingThread*               m_LoadingThread;

public:
    class CStage* GetStageObject()    const
    {
        return m_StageObject;
    }

    void SetStageObject(class CStage* Stage);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

private:
    void CreateMaterial();
    void CreateAnimationSequence();
    void CreatePlayerAnimationSequence();
    void CreateSkillAnimationSequence();
    void CreateMapAnimationSequence();
    void LoadSound();

public:
    void SetPerspective();

public:
    void CreateZakumAltarScene();

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

