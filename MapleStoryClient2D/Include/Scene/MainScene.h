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

private:
    void TestLoadScene();
};

