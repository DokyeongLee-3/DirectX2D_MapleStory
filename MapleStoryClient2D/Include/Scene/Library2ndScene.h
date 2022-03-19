#pragma once

#include "Scene/SceneMode.h"
//#include "../Widget/ConfigurationWindow.h"
//#include "../Widget/Inventory.h"
//#include "../Widget/SkillQuickSlotWindow.h"
//#include "../Widget/CharacterEXP.h"
//#include "../Widget/CharacterStatusWindow.h"
//#include "../Widget/BossMatching.h"
//#include "../Widget/StatWindow.h"
//#include "../Widget/DyingNoticeWindow.h"

class CLibrary2ndScene :
    public CSceneMode
{
public:
    CLibrary2ndScene();
    ~CLibrary2ndScene();

private:
    class CStage* m_StageObject;

private:
    //CSharedPtr<CConfigurationWindow>    m_ConfigurationWindow;
    //CSharedPtr<CInventory>              m_Inventory;
    //CSharedPtr<CSkillQuickSlotWindow>   m_SkillQuickSlot;
    //CSharedPtr<CCharacterStatusWindow>  m_CharacterStatusWindow;
    //CSharedPtr<CCharacterEXP>           m_CharacterEXPWindow;
    //CSharedPtr<CBossMatching>           m_BossMatchingWindow;
    //CSharedPtr<CStatWindow>             m_StatWindow;
    //CSharedPtr<CDyingNoticeWindow>      m_DyingNoticeWindow;

    std::function<void(bool, float)>    m_LoadingFunction;
    class CLoadingThread* m_LoadingThread;

    std::list<class CLowerClassBook*>   m_LowerClassBookList;

public:
    void PushLowerClassBook(class CLowerClassBook* Book);

    class CStage* GetStageObject()    const
    {
        return m_StageObject;
    }

    void SetStageObject(class CStage* Stage);

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
    void DeleteLowerClassBook(const std::string& Name);
    void DeleteLowerClassBook(class CLowerClassBook* Monster);

public:
    void CreateLobbyScene();
    void CreateWayToZakumScene();

public:
    class CLowerClassBook* FindLowerClassBook(bool Right, const Vector3& MyPos, float DistXConstraint, float DistYConstraint);

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

