#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CStage :
    public CGameObject
{
    friend class CScene;

protected:
    CStage();
    CStage(const CStage& obj);
    virtual ~CStage();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;


public:
    class CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CStage* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);
};

