#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"
#include "MovingTile.h"

class CMovingTileMapComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CMovingTileMapComponent();
    CMovingTileMapComponent(const CMovingTileMapComponent& com);
    virtual ~CMovingTileMapComponent();

protected:
    CSharedPtr<CSpriteMesh> m_Mesh;
    // 3종류의 용암 이미지를 모두 갖고 있는 아틀라스 이미지의 Material
    CSharedPtr<CMaterial> m_Material;
    std::vector<CMovingTile*> m_vecMovingTile;
    std::vector<MovingTileInfo>   m_vecMovingTileInfo;

    class CMovingTileMapConstantBuffer* m_CBuffer;
    class CStructuredBuffer* m_MovingTileInfo;

    // 흐르는 용암 개수
    int m_Count;
    Vector2 m_Speed;
    // 움직이다가 화면에 가려진걸 다시 원래 위치로 복구시켜서 컨베이어 벨트처럼 보이고 싶을때, 
    // 화면에 다 가려진 순간 MovingTile을 여기 위치로 다시 복구시킨다
    Vector2     m_RepeatPos;

    // CMovintTile을 움직이다가 화면에 가려진걸 다시 원래 위치로 복구시켜서 컨베이어 벨트처럼 보이게 하느냐 마느냐
    bool        m_IsRepeat;

public:
    void SetOwner(class CGameObject* Owner);
    void SetRepeatPos(const Vector2& Pos);

public:
    Vector2 GetRepeatPos()  const
    {
        return m_RepeatPos;
    }

    bool IsRepeat() const
    {
        return m_IsRepeat;
    }

public:
    void SetMaterial(CMaterial* Material);
    void SetDefaultFrame(const Vector2& Start, const Vector2& End);
    void SetDefaultFrame(float StartX, float StartY, float EndX, float EndY);
    void SetFrame(int Index, const Vector2& Start, const Vector2& End);
    void SetFrame(int Index, float StartX, float StartY, float EndX, float EndY);
    void SetOpacity(int Index, float Opacity);
    void SetCount(int Count);
    void SetSpeed(const Vector2& Speed);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CMovingTileMapComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void CreateMovingTile(int Count, const Vector2& Size);
    void CreateStructuredBuffer();
};

