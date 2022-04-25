#pragma once

#include "../Ref.h"


class CSceneMode :
    public CRef
{
    friend class CScene;

protected:
    CSceneMode();
    virtual ~CSceneMode();

protected:
    class CScene* m_Scene;
    CSharedPtr<class CGameObject>   m_PlayerObject;
    std::list<Vector3>                  m_DeadPos;

public:
    void PushDeadPos(const Vector3& Pos);

public:
    void SetPlayerObject(class CGameObject* Obj);

    class CGameObject* GetPlayerObject()    const
    {
        return m_PlayerObject;
    }

    class CScene* GetScene()  const;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

public:
    // 에디터에서 재정의해서 에디터의 AddObjectList함수만 동작하고
    // 엔진이나 Client에서는 아무런 동작하지 않도록 한다
    virtual void AddObjectList(const char* ObjName);
    // 에디터에서 재정의해서 에디터의 AddColliderList함수만 동작하고
    // 엔진이나 Client에서는 아무런 동작하지 않도록 한다
    virtual void AddComponentList(const char* ComponentName);
};

