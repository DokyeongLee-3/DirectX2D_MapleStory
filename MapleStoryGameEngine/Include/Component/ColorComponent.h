#pragma once
#include "SceneComponent.h"

class CColorComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CColorComponent();
    CColorComponent(const CColorComponent& com);
    virtual ~CColorComponent();

protected:
    CSharedPtr<class CColorMesh> m_Mesh;
    CSharedPtr<class CMaterial> m_Material;

public:
    void SetMaterial(CMaterial* Material)
    {
        m_Material = Material;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CColorComponent* Clone();
};

