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
    // �״� �ִϸ��̼����̸� �Ѿ��浹ü�� �浹�Ǿ �ٽ� �״� �ִϸ��̼��� ó������ �����ϵ��� ���� ���ϰ� �ϱ� ���ؼ�
    // ó�� �״� �ִϸ��̼� ���۽� m_IsChanging�� true���ϰ�, �̰� true�� �״� �ִϸ��̼��� �������� ���Ѵ�
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

