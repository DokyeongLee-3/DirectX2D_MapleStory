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
    // 3������ ��� �̹����� ��� ���� �ִ� ��Ʋ�� �̹����� Material
    CSharedPtr<CMaterial> m_Material;
    std::vector<CMovingTile*> m_vecMovingTile;
    std::vector<MovingTileInfo>   m_vecMovingTileInfo;

    class CMovingTileMapConstantBuffer* m_CBuffer;
    class CStructuredBuffer* m_MovingTileInfo;

    // �帣�� ��� ����
    int m_Count;
    Vector2 m_Speed;
    // �����̴ٰ� ȭ�鿡 �������� �ٽ� ���� ��ġ�� �������Ѽ� �����̾� ��Ʈó�� ���̰� ������, 
    // ȭ�鿡 �� ������ ���� MovingTile�� ���� ��ġ�� �ٽ� ������Ų��
    Vector2     m_RepeatPos;

    // CMovintTile�� �����̴ٰ� ȭ�鿡 �������� �ٽ� ���� ��ġ�� �������Ѽ� �����̾� ��Ʈó�� ���̰� �ϴ��� ������
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

