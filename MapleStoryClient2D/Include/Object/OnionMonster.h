#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"
#include "../ClientManager.h"

class COnionMonster :
    public CGameObject
{
    friend class CScene;

protected:
    COnionMonster();
    COnionMonster(const COnionMonster& obj);
    virtual ~COnionMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderCircle>     m_Body;
    CSharedPtr<CWidgetComponent>    m_DamageWidgetComponent;

    MonsterInfo                     m_MonsterInfo;
    // 죽는 애니메이션중이면 총알충돌체랑 충돌되어도 다시 죽는 애니메이션을 처음부터 시작하도록 변경 못하게 하기 위해서
    // 처음 죽는 애니메이션 시작시 m_IsChanging을 true로하고, 이게 true면 죽는 애니메이션을 시작하지 못한다
    bool    m_IsChanging;
    Monster_State                   m_MonsterState;
    float                           m_AccTime;
    float                           m_FiniteStateTimeTable[(int)Monster_State::End];

    //CSharedPtr<CPaperBurnComponent>   m_PaperBurn;

public:
    virtual void SetDamage(float Damage, bool Critical = false);
    void PushDamageFont(float Damage, bool Critical = false);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual COnionMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    void Die();
    void ReturnIdle();
    void FiniteState(float DeltaTime);

    void SetMonsterState(const Monster_State& State)
    {
        m_MonsterState = State;
    }

    Monster_State   GetMonsterState()   const
    {
        return m_MonsterState;
    }

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    //void CollisionStayCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);
};

