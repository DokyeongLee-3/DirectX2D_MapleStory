#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/StaticMesh.h"
#include "../Resource/Material/Material.h"

// SpriteComponent는 기본으로 SpriteMesh를 사용하게 되어 있는데
// 이건 DragObj가 사용할 용도의 Component라서
// 따로 만들어준(Index버퍼가 없는) 메쉬를 써야하므로 따로
// 그 용도의 Component를 만들어줌
class CStaticMeshComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CStaticMeshComponent();
    CStaticMeshComponent(const CStaticMeshComponent& com);
    virtual ~CStaticMeshComponent();

protected:
    CSharedPtr<CStaticMesh> m_Mesh;
    CSharedPtr<CMaterial> m_Material;

public:
    CMaterial* GetMaterial()    const
    {
        return m_Material;
    }

public:
    void SetMesh(const std::string& Name);
    void SetMesh(CStaticMesh* Mesh);
    void SetMaterial(CMaterial* Material);

public:
    void SetBaseColor(const Vector4& Color);
    void SetBaseColor(float r, float g, float b, float a);
    void SetRenderState(class CRenderState* State);
    void SetRenderState(const std::string& Name);
    void SetTransparency(bool Enable);
    void SetOpacity(float Opacity);
    void AddOpacity(float Opacity);

public:
    void AddTexture(int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void AddTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void AddTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void AddTexture(int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);


    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, class CTexture* Texture);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName, const std::string& PathName = TEXTURE_PATH);
    void SetTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath);
    void SetTexture(int Index, int Register, int ShaderType, const std::string& Name, const std::vector<TCHAR*>& vecFileName, const std::string& PathName = TEXTURE_PATH);

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CStaticMeshComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

