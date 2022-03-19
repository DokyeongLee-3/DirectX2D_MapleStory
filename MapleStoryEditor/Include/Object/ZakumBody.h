#pragma once
#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

class CZakumBody :
    public CMonster
{
    friend class CScene;

protected:
    CZakumBody();
    CZakumBody(const CZakumBody& obj);
    virtual ~CZakumBody();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;

    CSharedPtr<CSceneComponent>     m_LeftArm1Root;
    CSharedPtr<CSpriteComponent>    m_LeftArm1Lower;
    CSharedPtr<CSpriteComponent>    m_LeftArm1Upper;
    CSharedPtr<CSpriteComponent>    m_LeftArm1Hand;

    CSharedPtr<CSceneComponent>     m_LeftArm2Root;
    CSharedPtr<CSpriteComponent>    m_LeftArm2Lower;
    CSharedPtr<CSpriteComponent>    m_LeftArm2Upper;
    CSharedPtr<CSpriteComponent>    m_LeftArm2Hand;

    CSharedPtr<CSceneComponent>     m_LeftArm3Root;
    CSharedPtr<CSpriteComponent>    m_LeftArm3Lower;
    CSharedPtr<CSpriteComponent>    m_LeftArm3Upper;
    CSharedPtr<CSpriteComponent>    m_LeftArm3Hand;

    CSharedPtr<CSceneComponent>     m_LeftArm4Root;
    CSharedPtr<CSpriteComponent>    m_LeftArm4Lower;
    CSharedPtr<CSpriteComponent>    m_LeftArm4Upper;
    CSharedPtr<CSpriteComponent>    m_LeftArm4Hand;

    CSharedPtr<CSceneComponent>     m_RightArm1Root;
    CSharedPtr<CSpriteComponent>    m_RightArm1Lower;
    CSharedPtr<CSpriteComponent>    m_RightArm1Upper;
    CSharedPtr<CSpriteComponent>    m_RightArm1Hand;

    CSharedPtr<CSceneComponent>     m_RightArm2Root;
    CSharedPtr<CSpriteComponent>    m_RightArm2Lower;
    CSharedPtr<CSpriteComponent>    m_RightArm2Upper;
    CSharedPtr<CSpriteComponent>    m_RightArm2Hand;

    CSharedPtr<CSceneComponent>     m_RightArm3Root;
    CSharedPtr<CSpriteComponent>    m_RightArm3Lower;
    CSharedPtr<CSpriteComponent>    m_RightArm3Upper;
    CSharedPtr<CSpriteComponent>    m_RightArm3Hand;

    CSharedPtr<CSceneComponent>     m_RightArm4Root;
    CSharedPtr<CSpriteComponent>    m_RightArm4Lower;
    CSharedPtr<CSpriteComponent>    m_RightArm4Upper;
    CSharedPtr<CSpriteComponent>    m_RightArm4Hand;

    CSharedPtr<CColliderBox2D>     m_Body;


    bool                            m_AfterUpdate;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumBody* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

private:
    void NextAnimation(float DeltaTime);
};

