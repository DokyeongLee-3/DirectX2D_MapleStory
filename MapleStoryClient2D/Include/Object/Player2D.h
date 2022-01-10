#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"

class CPlayer2D :
    public CGameObject
{
    friend class CScene;
    friend class CPlayerAnimation2D;

protected:
    CPlayer2D();
    CPlayer2D(const CPlayer2D& obj);
    virtual ~CPlayer2D();

private:
    CSharedPtr<CSpriteComponent>    m_BodySprite;

    
    // ĳ���Ͱ� ���ǵ巣������ ��ų�� ����
    // �� Component ��ġ�� �ſ��� ����Ʈ�� ����� 
    // �� �ſ￡�� ȭ���� �߻�Ǵ� ����Ʈ ����
    CSharedPtr<CSceneComponent>     m_SylphideLancerMuzzle;
    CSharedPtr<CSpriteComponent>    m_SylphideLancerMirror;
    CSharedPtr<CSpriteComponent>    m_SkillBodyEffect;

    // ���̵� ������ ���� �����ϼ� �ִ� ���ݱ�ü
    CSharedPtr<CSpriteComponent>    m_VoidPressureAttackSphere;
    // ���̵� ������ ���� �÷��̾� �ٷ� �տ� ����� ���� ��ü
    CSharedPtr<CSpriteComponent>    m_VoidPressureSphere;

    CSharedPtr<CColliderBox2D>       m_Body;
    CSharedPtr<CCameraComponent>     m_Camera;

    float       m_Opacity;
    float       m_ScaleFactor;

public:
    CSceneComponent* GetSkillMuzzle()    const
    {
        return m_SylphideLancerMuzzle;
    }

    CSpriteComponent* GetRootSpriteComponent()    const
    {
        return m_BodySprite;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPlayer2D* Clone();

private:
    void Stand();
    void Stand(float DeltaTime);
    void MoveUp(float DeltaTime);
    void MoveDown(float DeltaTime);
    void RotationZInv(float DeltaTime);
    void RotationZ(float DeltaTime);
    void SylphideLancer(float DeltaTime);
    void VoidPressure(float DeltaTime);

    void Skill2(float DeltaTime);

public:
    void FlipAll(float DeltaTime);
};

