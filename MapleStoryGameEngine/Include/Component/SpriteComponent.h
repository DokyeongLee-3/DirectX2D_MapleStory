#pragma once

#include "SceneComponent.h"
#include "../Resource/Mesh/SpriteMesh.h"
#include "../Resource/Material/Material.h"


class CSpriteComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CSpriteComponent();
    CSpriteComponent(const CSpriteComponent& com);
    virtual ~CSpriteComponent();

protected:
    CSharedPtr<CSpriteMesh> m_Mesh;
    CSharedPtr<CMaterial> m_Material;
    class CAnimationSequence2DInstance* m_Animation;
    bool m_Flip;


public:
    bool IsFlip()   const
    {
        return m_Flip;
    }

    CMaterial* GetMaterial()    const
    {
        return m_Material;
    }

    float GetOpacity()  const
    {
        return m_Material->GetOpacity();
    }

    class CAnimationSequence2DInstance* GetAnimationInstance()  const;
    class CAnimationSequence2DData* GetCurrentAnimation()    const;

    const std::string& GetCurrentAnimationName()    const;

    void ChangeAnimation(const std::string& AnimationName);

    void SetMaterial(CMaterial* Material);
    void SetMesh(const std::string& Name);

public:
    void SetBaseColor(const Vector4& Color);
    void SetBaseColor(float r, float g, float b, float a);
    Vector4 GetBaseColor() const;
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
    virtual CSpriteComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    void Flip();

public:
    template <typename T>
    void CreateAnimationInstance()
    {
        T* Anim = new T;

        Anim->SetScene(m_Scene);
        Anim->SetOwner(this);

        if (!Anim->Init())
        {
            SAFE_DELETE(Anim);
            return;
        }

        // 하나의 AnimationInstance를 갖고 있어야하니까
        // 기존에 갖고 있는게 있었다면 그걸 제거
        SAFE_DELETE(m_Animation);

        m_Animation = Anim;
    }

    template<typename T>
    void LoadAnimationInstance()
    {
        T* Anim = new T;
        Anim->SetScene(m_Scene);
        Anim->SetOwner(this);

        SAFE_DELETE(m_Animation);
        m_Animation = Anim;
    }
};

