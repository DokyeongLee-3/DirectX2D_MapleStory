#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/WidgetComponent.h"
#include "Component/PaperBurnComponent.h"

class COnionMonster :
    public CGameObject
{
    friend class CScene;

protected:
    COnionMonster();
    COnionMonster(const COnionMonster& obj);
    virtual ~COnionMonster();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderCircle>     m_Body;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual COnionMonster* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    CSpriteComponent* GetSpriteComponent()    const
    {
        return m_Sprite;
    }

};

