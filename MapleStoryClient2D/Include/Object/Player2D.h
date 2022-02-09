#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "../Client.h"

struct SecondPhaseSkillInfo
{
    int LevelSylphideLancer;
    int LevelVoidPressure;

    SecondPhaseSkillInfo()
    {
        LevelSylphideLancer = 20;
        LevelVoidPressure = 20;
    }
};

struct ThirdPhaseSkillInfo
{
    int LevelNoxSpear;
    int LevelDeathSide;

    ThirdPhaseSkillInfo()
    {
        LevelNoxSpear = 10;
        LevelDeathSide = 5;
    }
};

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

    
    // 캐릭터가 실피드랜서같은 스킬을 쓰면
    // 이 Component 위치에 거울모양 이펙트가 생기고 
    // 이 거울에서 화살이 발사되는 이펙트 생성
    CSharedPtr<CSceneComponent>     m_SylphideLancerMuzzle;
    CSharedPtr<CSpriteComponent>    m_SylphideLancerMirror;
    CSharedPtr<CSpriteComponent>    m_SkillBodyEffect;

    // 보이드 프레셔 사용시 움직일수 있는 공격구체
    CSharedPtr<CSpriteComponent>    m_VoidPressureAttackSphere;
    // 보이드 프레셔 사용시 플레이어 바로 앞에 생기는 작은 구체
    CSharedPtr<CSpriteComponent>    m_VoidPressureSphere;

    CSharedPtr<CColliderBox2D>       m_Body;
    CSharedPtr<CCameraComponent>     m_Camera;

    CSharedPtr<CWidgetComponent>     m_SimpleHUDWidget;

    float       m_Opacity;
    float       m_ScaleFactor;

    PlayerInfo m_PlayerInfo;

public:
    CSceneComponent* GetSkillMuzzle()    const
    {
        return m_SylphideLancerMuzzle;
    }

    CSpriteComponent* GetRootSpriteComponent()    const
    {
        return m_BodySprite;
    }

    const PlayerInfo& GetInfo() const
    {
        return m_PlayerInfo;
    }

    CWidgetComponent*    GetWidgetComponent()    const
    {
        return m_SimpleHUDWidget;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPlayer2D* Clone();

public:
    virtual void SetScene(class CScene* Scene);

private:
    void MoveUp(float DeltaTime);
    void MoveDown(float DeltaTime);
    void RotationZInv(float DeltaTime);
    void RotationZ(float DeltaTime);
    void SylphideLancer(float DeltaTime);
    void VoidPressure(float DeltaTime);

    void Skill2(float DeltaTime);

public:
    void FlipAll(float DeltaTime);
    void GotoNextMap(float DeltaTime);
    void ProduceSecondSylphideLander(float DeltaTime);
};

