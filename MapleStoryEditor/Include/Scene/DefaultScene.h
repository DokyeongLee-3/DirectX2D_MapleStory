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

    void SetStageObject(class CStage* Stage)
    {
        m_StageObject = Stage;
    }

private:
    bool LoadAnimationSequence2D();
    void CameraRight(float DeltaTime);
    void CameraLeft(float DeltaTime);
    void CameraUp(float DeltaTime);
    void CameraDown(float DeltaTime);

public:
    virtual void AddObjectList(const char* ObjName);
    virtual void AddComponentList(const char* ComponentName);
};

