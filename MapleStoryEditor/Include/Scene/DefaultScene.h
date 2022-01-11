#pragma once

#include "Scene/SceneMode.h"

class CDefaultScene :
    public CSceneMode
{
public:
    CDefaultScene();
    ~CDefaultScene();

public:
    virtual bool Init();
    class CGameObject* m_CameraObject;
    class CStage*      m_StageObject;
    class CCameraComponent* m_CameraComponent;

public:
    class CStage* GetStageObject()    const
    {
        return m_StageObject;
    }

    void SetStageObject(class CStage* Stage);

private:
    bool LoadAnimationSequence2D();

private:
    void CameraRight(float DeltaTime);
    void CameraLeft(float DeltaTime);
    void CameraUp(float DeltaTime);
    void CameraDown(float DeltaTime);

private:
    void SelectObjectRight(float DeltaTime);
    void SelectObjectLeft(float DeltaTime);
    void SelectObjectUp(float DeltaTime);
    void SelectObjectDown(float DeltaTime);

public:
    virtual void AddObjectList(const char* ObjName);
    virtual void AddComponentList(const char* ComponentName);
};

