#pragma once

#include "SceneComponent.h"
#include "../Resource/Shader/ColliderConstantBuffer.h"

class CColliderComponent :
    public CSceneComponent
{
    friend class CGameObject;
    friend class CCollision;

protected:
    CColliderComponent();
    CColliderComponent(const CColliderComponent& com);
    virtual ~CColliderComponent();

protected:
    Collider_Type   m_ColliderType;
    Vector3         m_Offset;
    Vector3         m_Min;
    Vector3         m_Max;

    std::vector<int>    m_vecSectionIndex;
    // 이전 프레임에서 충돌한 충돌체들
    std::list<CColliderComponent*>  m_PrevCollisionList;
    // 현재 프레임에 이미 자신과 충돌처리가 된 충돌체 목록
    // 여기에 충돌체를 추가할때는 항상 추가하려는 충돌체가 리스트에 이미
    // 있는지 확인하고 추가해야하는데 예를 들어, 완벽하게 두 충돌체가 겹쳐있는데
    // 두 충돌체가 4개의 Section에 걸쳐있다면 4개의 Section 모두에서 충돌처리를 할 필요 없이 한 Section에서만
    // 하면 되기 때문이다. 때문이다. 따라서 어떤 충돌체와 이번 프레임에 처음 충돌했을때
    // 여기에 그 충돌체를 넣어주고, 다른 Section에서 충돌 로직을 돌때 이 List를 확인하고,
    // 여기에 있는 충돌체와 충돌했다면 또 충돌처리를 또 하지 않고 넘어가야 한다. 
    std::list<CColliderComponent*>  m_CurrentCollisionList; 
    CollisionProfile* m_Profile;
    // 이번 프레임에 Collider가 자신이 속한 Section이 배정되었는지 아닌지
    bool                m_CurrentSectionCheck;
    CollisionResult     m_Result;
    CollisionResult     m_MouseResult;
    std::list<std::function<void(const CollisionResult&)>>  m_CollisionCallback[(int)Collision_State::Max];
    std::list<std::function<void(const CollisionResult&)>>  m_CollisionMouseCallback[(int)Collision_State::Max];
    bool                m_MouseCollision;
    CSharedPtr<class CMesh> m_Mesh;
    CSharedPtr<class CShader> m_Shader;
    CColliderConstantBuffer* m_CBuffer;

public:
    Collider_Type GetColliderType() const
    {
        return m_ColliderType;
    }

    void SetOffset(const Vector3& Offset)
    {
        m_Offset = Offset;
    }

    void SetOffset(float x, float y, float z)
    {
        m_Offset = Vector3(x, y, z);
    }

    Vector3 GetOffset() const
    {
        return m_Offset;
    }

    Vector3 GetMin()    const
    {
        return m_Min;
    }

    Vector3 GetMax()    const
    {
        return m_Max;
    }

    CollisionResult GetCollisionResult()    const
    {
        return m_Result;
    }

    CollisionProfile* GetCollisionProfile() const
    {
        return m_Profile;
    }

    void AddSectionIndex(int Index)
    {
        m_vecSectionIndex.push_back(Index);
    }

    void CurrentSectionCheck()
    {
        m_CurrentSectionCheck = true;
    }

    bool GetCurrentSectionCheck()   const
    {
        return m_CurrentSectionCheck;
    }

    void SetCollisionProfile(const std::string& Name);
    void CheckPrevColliderSection();
    void AddPrevCollision(CColliderComponent* Collider);
    void DeletePrevCollision(CColliderComponent* Collider);
    bool EmptyPrevCollision();
    bool CheckPrevCollision(CColliderComponent* Collider);
    bool CheckPrevCollisionGameObjectType(size_t TypeID);
    bool CheckCurrentFrameCollision(CColliderComponent* Collider);
    void AddCurrentFrameCollision(CColliderComponent* Collider);
    void CallCollisionCallback(Collision_State State);
    void CallCollisionMouseCallback(Collision_State State);
    void ClearFrame();

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void CheckCollision();
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CColliderComponent* Dest) = 0;
    virtual bool CollisionMouse(const Vector2& MousePos) = 0;

public:
    template <typename T>
    void AddCollisionCallback(Collision_State State, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionCallback[(int)State].push_back(std::bind(Func, Obj, std::placeholders::_1));
    }

    template <typename T>
    void AddCollisionMouseCallback(Collision_State State, T* Obj, void(T::* Func)(const CollisionResult&))
    {
        m_CollisionMouseCallback[(int)State].push_back(std::bind(Func, Obj, std::placeholders::_1));
    }

public:
    template <typename T>
    T* CreateEmptyBuffer()
    {
        T* CBuffer = new T;

        m_CBuffer = CBuffer;

        m_CBuffer->SetColliderCBuffer();

        return CBuffer;
    }
};

