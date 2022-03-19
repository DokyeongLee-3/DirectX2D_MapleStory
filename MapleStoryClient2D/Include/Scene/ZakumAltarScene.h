#pragma once

#include "Scene/SceneMode.h"

class CZakumAltarScene :
    public CSceneMode
{
public:
    CZakumAltarScene();
    ~CZakumAltarScene();

private:
    class CStage* m_StageObject;

private:

    std::function<void(bool, float)>    m_LoadingFunction;
    class CLoadingThread* m_LoadingThread;

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
    void CreateParticle();
    void CreateAnimationSequence();
    void CreatePlayerAnimationSequence();
    void CreateMonsterAnimationSequence();
    void CreateSkillAnimationSequence();
    void CreateMapAnimationSequence();
    void LoadSound();

public:
    void SetPerspective();
    void AddFloorCollider();

public:
    void CreateWayToZakumScene();

public:
    template <typename T>
    void SetLoadingFunction(T* Obj, void(T::* Func)(bool, float))
    {
        m_LoadingFunction = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
    }
};

