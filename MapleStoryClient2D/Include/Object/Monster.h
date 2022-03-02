#pragma once

#include "GameObject/GameObject.h"
#include "Component/WidgetComponent.h"
#include "../ClientManager.h"

class CMonster :
    public CGameObject
{
    friend class CScene;

protected:
    CMonster();
    CMonster(const CMonster& obj);
    virtual ~CMonster();

protected:
    CSharedPtr<CWidgetComponent>    m_DamageWidgetComponent;

    MonsterInfo                     m_MonsterInfo;
    // 죽는 애니메이션중이면 총알충돌체랑 충돌되어도 다시 죽는 애니메이션을 처음부터 시작하도록 변경 못하게 하기 위해서
    // 처음 죽는 애니메이션 시작시 m_IsChanging을 true로하고, 이게 true면 죽는 애니메이션을 시작하지 못한다
    bool    m_IsChanging;
    Monster_State                   m_MonsterState;
    float                           m_AccTime;
    float                           m_FiniteStateTimeTable[(int)Monster_State::End];

public:
    virtual void SetDamage(float Damage, bool Critical = false);
    virtual void PushDamageFont(float Damage, bool Critical = false);

public:
    void SetMonsterState(const Monster_State& State)
    {
        m_MonsterState = State;
    }

    Monster_State   GetMonsterState()   const
    {
        return m_MonsterState;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

    void Die();
};

