#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"

struct PlayerInfo
{
    TCHAR Name[256];
    TCHAR Job[32];
    int Level;
    int HPMax;
    int HP;
    int MPMax;
    int MP;
    int STR;
    int INT;
    int DEX;
    int LUK;
    int EXPMax;
    int EXP;
    int Meso;

    PlayerInfo() :
        Name(TEXT("가막못의오리")),
        Job(TEXT("루미너스")),
        Level(60),
        HPMax(1000),
        HP(700),
        MPMax(15000),
        MP(15000),
        STR(4),
        INT(338),
        DEX(4),
        LUK(4),
        EXPMax(Level * 1000),
        EXP(10000),
        Meso(0)
    {
    }
};

enum class PlayerDir
{
    None,
    Left,
    Right,
    Up,
    Down
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
    CSharedPtr<CSpriteComponent>    m_PlayerOrb;
    CSharedPtr<CSpriteComponent>    m_SylphideLancerMirror;
    CSharedPtr<CSpriteComponent>    m_SkillBodyEffect;

    CSharedPtr<CColliderBox2D>      m_Body;
    CSharedPtr<CCameraComponent>    m_Camera;

    CSharedPtr<CWidgetComponent>    m_DamageWidgetComponent;
    CSharedPtr<CWidgetComponent>    m_NameWidgetComponent;

    class CVoidPressure*            m_VoidPressure;
    class CVoidPressureOrb*         m_VoidPressureOrb;
    class CPlayerSkillInfo*         m_PlayerSkillInfo;
    class CStage*                   m_CurrentStage;

    float       m_Opacity;
    bool        m_IsFlip;
    bool        m_IsChanging;
    bool        m_OnVoidPressure;
    bool        m_OnLightTransforming;
    bool        m_OnJump;
    bool        m_OnKnockBack;
    bool        m_OnKnockBackLeft;
    float       m_OnKnockBackTime;
    float       m_OnKnockBackAccTime;
    float       m_JumpForce;
    float       m_HitOpacity;

    // 몬스터와 충돌했을때 깜빡거리는 효과를 시작할건지 말건지
    bool        m_OnHit;
    float       m_OnHitTime;
    float       m_OnHitAccTime;

    bool        m_OnLope;
    // 로프 가장 위까지 전부 타고 올라가면 m_Gravity = true로 만들어서 떨어져서 바닥타일과 충돌하도록 유도하는데
    // 그때도 위 방향키 계속 누르고있으면 떨어지고, 다시 로프타고 이걸 반복해서 그걸 제어해주는 bool 변수
    bool        m_LopeEnable;
    // 로프에서 메달려있다가 점프한건지
    bool        m_LopeJump;
    // 플레이어와 충돌하고 있는 충돌체(ex. StaticMapObj, TileObj)들의 ID
    std::list<int>                  m_ListCollisionID;

    PlayerDir   m_Dir;
    PlayerInfo  m_PlayerInfo;

    Vector3		m_PrevFrameWorldPos;
    Vector3		m_CurrentFrameMove;

    bool        m_Dead;
    Vector3     m_GhostVector;

    int         m_PrevSameTileObjColliderCount;

    Vector2     m_PrevFrameCameraMove;
    Vector2     m_CurrentFrameCameraMove;

    bool        m_OnXCameraShake;
    bool        m_OnYCameraShake;
    float       m_CameraShakeFrequency;
    float       m_CameraShakeTime;
    float       m_AccCameraShakeTime;
    float       m_AccCameraShakeSingleDirTime;
    Vector3     m_OriginRelativeCamPos;
    Vector2     m_CameraShakeDir;

    bool m_ProduceLatterGroup;

public:
    class CPlayerSkillInfo* GetPlayerSkillInfo()   const;

    bool IsProduceLatterGroup() const
    {
        return m_ProduceLatterGroup;
    }

    void SetProduceLatterGroup(bool Produce)
    {
        m_ProduceLatterGroup = Produce;
    }

    Vector2 GetCurrentFrameCameraMove() const
    {
        return m_CurrentFrameCameraMove;
    }

    void ClearListCollision()
    {
        m_ListCollisionID.clear();
    }

    void SetChanging(bool Changing)
    {
        m_IsChanging = Changing;
    }

    bool IsChanging()   const
    {
        return m_IsChanging;
    }

    Vector3	GetCurrentFrameMove()	const
    {
        return m_CurrentFrameMove;
    }

    void AddCollisionID(int ID)
    {
        m_ListCollisionID.push_back(ID);
    }

    bool IsCollisionIDEmpty()   const
    {
        return m_ListCollisionID.empty();
    }

    bool CheckCollisionID(int ID)
    {
        auto iter = m_ListCollisionID.begin();
        auto iterEnd = m_ListCollisionID.end();

        for (; iter != iterEnd; ++iter)
        {
            if ((*iter) == ID)
                return true;
        }

        return false;
    }

    void EraseCollisionID(int ID)
    {
        auto iter = m_ListCollisionID.begin();
        auto iterEnd = m_ListCollisionID.end();

        for (; iter != iterEnd; ++iter)
        {
            if ((*iter) == ID)
            {
                m_ListCollisionID.erase(iter);
                return;
            }

        }
    }

    bool GetOnKnockBack()   const
    {
        return m_OnKnockBack;
    }

    float GetJumpForce()    const
    {
        return m_JumpForce;
    }

    float GetHitOpacity()   const
    {
        return m_HitOpacity;
    }

    bool GetIsLopeJump()    const
    {
        return m_LopeJump;
    }

    void SetLopeJump(bool LopeJump)
    {
        m_LopeJump = LopeJump;
    }

    void SetHitOpacity(float Opacity)
    {
        m_Opacity = Opacity;
    }

    void SetOnJump(bool Jump)
    {
        m_OnJump = Jump;
    }

    void SetOnKnockBack(bool KnockBack)
    {
        m_OnKnockBack = KnockBack;
    }

    void SetOnKnockBackAccTime(float Time)
    {
        m_OnKnockBackAccTime = Time;
    }

    bool GetOnJump()    const
    {
        return m_OnJump;
    }

    void SetDir(PlayerDir Dir)
    {
        m_Dir = Dir;
    }

    void SetOnLope(bool OnLope)
    {
        m_OnLope = OnLope;
    }

    bool IsOnLope() const
    {
        return m_OnLope;
    }

    CSpriteComponent* GetRootSpriteComponent()    const
    {
        return m_BodySprite;
    }

    CColliderBox2D* GetPlayerBody() const
    {
        return m_Body;
    }

    const PlayerInfo& GetInfo() const
    {
        return m_PlayerInfo;
    }

    class CVoidPressureOrb* GetVoidPressureOrb()    const;

    CWidgetComponent* GetDamageWidgetComponent()  const
    {
        return m_DamageWidgetComponent;
    }

    CWidgetComponent* GetNameWidgetComponent()  const
    {
        return m_NameWidgetComponent;
    }

    bool IsDead()   const
    {
        return m_Dead;
    }

    void SetOnHit(bool OnHit)
    {
        m_OnHit = OnHit;
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
    void MoveLeft(float DeltaTime);
    void MoveRight(float DeltaTime);
    void Jump(float DeltaTime);
    void SylphideLancer(float DeltaTime);
    void VoidPressure(float DeltaTime);
    void LightTransforming(float DeltaTime);
    void DeathSide(float DeltaTime);
    void UseNum1QuickSlotItem(float DeltaTime);


public:
    void SetVoidPressure(class CVoidPressure* VoidPressure);
    void SetVoidPressureOrb(class CVoidPressureOrb* VoidPressureOrb);

public:
    virtual void FlipAll(float DeltaTime);
    void GotoNextMap(float DeltaTime);
    void GotoZakumAltar();
    void ProduceSecondSylphideLander(float DeltaTime);
    void EffectEnd(float DeltaTime);
    void ReturnIdle(float DeltaTime);
    void RopeActionStop(float DeltaTime);
    void ReturnFadeApply();
    void ReturnAlive();

public:
    void VoidPressureCancle(float DeltaTime);
    void CollisionBeginCallback(const CollisionResult& Result);
    //void CollisionStayCallback(const CollisionResult& Result);
    void CollisionEndCallback(const CollisionResult& Result);

public:
    void CameraTrack();
    void KnockBack(float DeltaTime);
    virtual void SetDamage(float Damage, bool Critical = false);
    void PushDamageFont(float Damage);
    void PickItem(float DeltaTime);
    void FallTomb();

public:
    void GetEXP(int EXP);
    void LevelUp(float DeltaTime);
    void UpSTR(int Count);
    void UpDEX(int Count);
    void UpINT(int Count);
    void UpLUK(int Count);
    void Die();
    void DeadRound();
   // void MovePointDown(float DeltaTime);
    //void PathResult(const std::list<Vector3>& PathList);

public:
    void SetXCameraShake(bool Shake);
    void SetYCameraShake(bool Shake);
    void CameraShake(float DeltaTime);

public:
    static bool SortCollider(CColliderBox2D* Src, CColliderBox2D* Dest)
    {
        return Src->GetInfo().Center.y < Dest->GetInfo().Center.y;
    }
};

