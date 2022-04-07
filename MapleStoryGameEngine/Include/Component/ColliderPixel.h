#pragma once

#include "ColliderComponent.h"
#include "../Resource/Mesh/Mesh.h"

class CColliderPixel :
    public CColliderComponent
{
    friend class CGameObject;

protected:
    CColliderPixel();
    CColliderPixel(const CColliderPixel& com);
    virtual ~CColliderPixel();

protected:
    // 여기 안에 박스 충돌체는 Box2D메시를 사용하고, Shader는
    // ColliderComponent에서 공통적으로 사용하는 ColliderShader를 사용
    PixelInfo   m_Info;
    // 픽셀 충돌체 이미지 자체에 대한 메시.
    // SpriteMesh를 사용하는데 중점이 좌하단이어서 셰이더에서
    // 가로 0.5, 세로 0.5만큼 4개의 정점들을 왼쪽,아래로 이동시켜서
    // 중앙을 중점으로 만들어줘서 사용하고, m_PixelShader에서 사용한다
    CSharedPtr<class CMesh> m_PixelMesh;
    CSharedPtr<class CShader> m_PixelShader;


public:
    const std::string& GetMeshName()    const
    {
        return m_Mesh->GetName();
    }

public:
    PixelInfo GetInfo() const
    {
        return m_Info;
    }

    void SetInfo(const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoFullPath(const TCHAR* FullPath);
    void SetInfoMultibyte(const char* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetInfoMultibyteFullPath(const char* FullPath);
    void SetPixelCollisionType(PixelCollision_Type Type);
    void SetPixelColor(unsigned char r, unsigned char g, unsigned char b);
    void SetPixelAlpha(unsigned char a);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColliderPixel* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
    virtual bool Collision(CColliderComponent* Dest);
    virtual bool CollisionMouse(const Vector2& MousePos);
};

