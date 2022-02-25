#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"

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
    float   m_HP;
    // �״� �ִϸ��̼����̸� �Ѿ��浹ü�� �浹�Ǿ �ٽ� �״� �ִϸ��̼��� ó������ �����ϵ��� ���� ���ϰ� �ϱ� ���ؼ�
    // ó�� �״� �ִϸ��̼� ���۽� m_IsChanging�� true���ϰ�, �̰� true�� �״� �ִϸ��̼��� �������� ���Ѵ�
    bool    m_IsChanging;

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
    void CollisionCallback(const CollisionResult& result);

public:
    void Die();
    void ReturnIdle();
};

