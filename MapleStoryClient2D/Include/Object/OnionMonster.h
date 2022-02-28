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
    // �״� �ִϸ��̼����̸� �Ѿ��浹ü�� �浹�Ǿ �ٽ� �״� �ִϸ��̼��� ó������ �����ϵ��� ���� ���ϰ� �ϱ� ���ؼ�
    // ó�� �״� �ִϸ��̼� ���۽� m_IsChanging�� true���ϰ�, �̰� true�� �״� �ִϸ��̼��� �������� ���Ѵ�
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

