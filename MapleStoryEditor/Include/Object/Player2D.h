#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CPlayer2D :
    public CGameObject
{
    friend class CScene;

protected:
    CPlayer2D();
    CPlayer2D(const CPlayer2D& obj);
    virtual ~CPlayer2D();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CSpriteComponent>    m_OrbSprite;
    CSharedPtr<CSpriteComponent>    m_BodyEffectSprite;
    CSharedPtr<CColliderBox2D>      m_Body;

    float                           m_TransparencyTime;
    float                           m_AccTransparencyTime;
    bool                            m_IsTransParent;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPlayer2D* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

private:
    void MoveUp(float DeltaTime);
    void MoveDown(float DeltaTime);
    void RotationZInv(float DeltaTime);
    void RotationZ(float DeltaTime);

private:
    void AddMotionAnimation();
    void AddSkillAnimation();

private:
    void FlipAll(float DeltaTime);
    void NextAnimation(float DeltaTime);

public:
    void TransformingLight(float DeltaTime);
    bool IsMouseCollision();
    void ReturnToBlank();
};

