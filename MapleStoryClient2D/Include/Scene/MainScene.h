#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/MainWidget.h"


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

private:
    void CreateMaterial();
    void CreateAnimationSequence();
    void CreatePlayerAnimationSequence();
    void CreateSkillAnimationSequence();
    void CreateMonsterAnimationSequence();
};

