#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

// 애니메이션없는 맵상에 배치되는 모든 오브젝트 종류들
class CStaticMapObj :
    public CGameObject
{
    friend class CScene;

public:
    CStaticMapObj();
    CStaticMapObj(const CStaticMapObj& obj);
    virtual ~CStaticMapObj();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
  
    int                             m_CollisionID; // 플레이어가 여러 StaticMapObj와 충돌할때 충돌 처리시 구별을 위한 ID
    bool                            m_IsFloor;

public:
    bool    IsFloor()   const
    {
        return m_IsFloor;
    }

    void SetIsFloor(bool IsFloor)
    {
        m_IsFloor = IsFloor;
    }

    class CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

    void SetCollisionID(int ID)
    {
        m_CollisionID = ID;
    }

    int GetCollisionID()    const
    {
        return m_CollisionID;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CStaticMapObj* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);

public:
    virtual void ReturnMemory(void* Mem);
    virtual void ResetInfo();
};

