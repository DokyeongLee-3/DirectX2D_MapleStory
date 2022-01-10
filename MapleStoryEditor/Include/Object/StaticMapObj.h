#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

// 애니메이션없는 맵상에 배치되는 모든 오브젝트 종류들
class CStaticMapObj :
    public CGameObject
{
    friend class CScene;

protected:
    CStaticMapObj();
    CStaticMapObj(const CStaticMapObj& obj);
    virtual ~CStaticMapObj();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>      m_DragCollider;

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
    virtual CStaticMapObj* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

