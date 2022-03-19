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
    CSharedPtr<CSpriteComponent>    m_BodySprite;

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

    /*CSharedPtr<CColliderBox2D>    m_LeftArm1Collider;
    CSharedPtr<CColliderBox2D>    m_LeftArm2Collider;*/

    CSharedPtr<CColliderBox2D>     m_Body;

    float                           m_AccTime;
    std::vector<float>              m_vecAccRotation;

    Zakum_State                     m_ZakumState;
    float                           m_IdleStateTime;

    std::vector<Zakum_ArmState>     m_vecArmState;
    std::vector<float>              m_vecShakeRotBound;
    std::vector<float>              m_vecShakeRotSign;
    std::list<int>	                m_ListAttackArmNum;
    std::vector<float>              m_vecUpHandRotUpperBound;
    std::vector<float>              m_vecAccUpHandRot;
    std::vector<Vector3>            m_vecPreviousHandPos;
    std::vector<Vector3>            m_vecPreviousHandRot;
    std::vector<Vector3>            m_vecPreviousLowerPos;
    std::vector<Vector3>            m_vecPreviousLowerRot;
    std::vector<Vector3>            m_vecPreviousUpperPos;
    std::vector<Vector3>            m_vecPreviousUpperRot;
    std::vector<Vector3>            m_vecAttackWarningPos;
    std::vector<float>              m_vecSmashRotUpperBound;

    int m_SmashCount;
    int m_SmashCountLimit;
    int m_ClapCount;
    int m_ClapCountLimit;

    int	                            m_ClapArmNum;
    bool                            m_HandMeet;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumBody* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void SetHandMeet(bool Meet)
    {
        m_HandMeet = Meet;
    }

private:
    void UpHand(int ArmID, float DeltaTime);
    void CheckUpHandAttackArm(float DeltaTime);
    
public:
    void ArmSmash(int ArmID);
    // 팔을 내려치고 다시 원래 위치로 돌아오게하는 함수
    void ReturnArm(int ArmID);
    void ReturnArmOriginPos();

public:
    void StretchArm(int ArmID, float DeltaTime);
    void PickAndStretchClapHand(float DeltaTime);
    void ZakumClap();
    void CreateClapEffect();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_BodySprite;
    }

};

