#pragma once
#include "Monster.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

struct ZakumInfo
{
    int BodyHP;
    int BodyHPMax;
    int LeftArm1HP;
    int LeftArm2HP;
    int LeftArm3HP;
    int LeftArm4HP;
    int RightArm1HP;
    int RightArm2HP;
    int RightArm3HP;
    int RightArm4HP;

    ZakumInfo() :
        BodyHP(100000),
        BodyHPMax(300000),
        LeftArm1HP(30000),
        LeftArm2HP(30000),
        LeftArm3HP(30000),
        LeftArm4HP(30000),
        RightArm1HP(30000),
        RightArm2HP(30000),
        RightArm3HP(30000),
        RightArm4HP(30000)
    {
    }
};

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

    CSharedPtr<CColliderBox2D>      m_LeftArm1HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm2HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm3HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm4HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm1HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm2HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm3HandAttackCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm4HandAttackCollider;

    /*CSharedPtr<CColliderBox2D>    m_LeftArm1Collider;
    CSharedPtr<CColliderBox2D>    m_LeftArm2Collider;*/

    CSharedPtr<CColliderBox2D>     m_Body;
    CSharedPtr<CColliderBox2D>      m_LeftArm1HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm2HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm3HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_LeftArm4HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm1HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm2HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm3HandDefenseCollider;
    CSharedPtr<CColliderBox2D>      m_RightArm4HandDefenseCollider;

    CSharedPtr<CWidgetComponent>    m_LeftArm1HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_LeftArm2HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_LeftArm3HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_LeftArm4HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_RightArm1HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_RightArm2HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_RightArm3HandDamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_RightArm4HandDamageWidgetComponent;

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
    std::vector<float>              m_vecClapRotUpperBound;

    int m_SmashCount;
    int m_SmashCountLimit;
    int m_ClapCount;
    int m_ClapCountLimit;

    int	                            m_ClapArmNum;
    bool                            m_HandMeet;
    ZakumInfo                       m_ZakumInfo;
    int                             m_AliveArmCount;
    int                             m_CurrentCollisionArmID;

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

    void SetCurrentCollisionID(int ID)
    {
        m_CurrentCollisionArmID = ID;
    }

    const ZakumInfo& GetZakumInfo() const
    {
        return m_ZakumInfo;
    }

private:
    void UpHand(int ArmID, float DeltaTime);
    void CheckUpHandAttackArm(float DeltaTime);
    void DecreaseAliveArmCount();

public:
    void ArmSmash(int ArmID);
    // 팔을 내려치고 다시 원래 위치로 돌아오게하는 함수
    void SmashReturnArm(int ArmID);
    void SmashReturnArmOriginPos();

public:
    void StretchArm(int ArmID, float DeltaTime);
    void PickAndStretchClapHand(float DeltaTime);
    void ZakumClap();
    void CreateClapEffect();
    void ClapReturnArm();
    void ClapReturnArmOriginPos();

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_BodySprite;
    }

public:
    void CollisionBeginCallback(const CollisionResult& Result);
    virtual void SetDamage(float Damage, bool Critical = false);
    virtual void PushDamageFont(float Damage, bool Critical = false);
};

