#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/MainWidget.h"
#include "../Widget/ConfigurationWindow.h"

class CMainScene :
    public CSceneMode
{
public:
    CMainScene();
    ~CMainScene();

public:
    virtual bool Init();

private:
    CSharedPtr<CMainWidget> m_MainWidget;
    CSharedPtr<CConfigurationWindow> m_ConfigurationWindow;
    class CStage* m_StageObject;
    std::function<void(bool, float)> m_LoadingFunction;

public:
    class CStage* GetStageObject()    const
    {
        return m_StageObject;
    }

    void SetStageObject(class CStage* Stage);

private:
    void CreateMaterial();
    void CreateAnimationSequence();
    void CreatePlayerAnimationSequence();
    void CreateSkillAnimationSequence();
    void CreateMonsterAnimationSequence();
    void CreateParticle();

private:
    void TestLoadScene();

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

