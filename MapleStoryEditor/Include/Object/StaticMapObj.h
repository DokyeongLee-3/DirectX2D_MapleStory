#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

// �ִϸ��̼Ǿ��� �ʻ� ��ġ�Ǵ� ��� ������Ʈ ������
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

