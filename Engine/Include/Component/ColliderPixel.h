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
    // ���� �ȿ� �ڽ� �浹ü�� Box2D�޽ø� ����ϰ�, Shader��
    // ColliderComponent���� ���������� ����ϴ� ColliderShader�� ���
    PixelInfo   m_Info;
    // �ȼ� �浹ü �̹��� ��ü�� ���� �޽�.
    // SpriteMesh�� ����ϴµ� ������ ���ϴ��̾ ���̴�����
    // ���� 0.5, ���� 0.5��ŭ 4���� �������� ����,�Ʒ��� �̵����Ѽ�
    // �߾��� �������� ������༭ ����ϰ�, m_PixelShader���� ����Ѵ�
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

