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

public:
    void SetPlayerObject(class CGameObject* Obj);

    class CGameObject* GetPlayerObject()    const
    {
        return m_PlayerObject;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);

public:
    // �����Ϳ��� �������ؼ� �������� AddObjectList�Լ��� �����ϰ�
    // �����̳� Client������ �ƹ��� �������� �ʵ��� �Ѵ�
    virtual void AddObjectList(const char* ObjName);
    // �����Ϳ��� �������ؼ� �������� AddColliderList�Լ��� �����ϰ�
    // �����̳� Client������ �ƹ��� �������� �ʵ��� �Ѵ�
    virtual void AddComponentList(const char* ComponentName);
};

