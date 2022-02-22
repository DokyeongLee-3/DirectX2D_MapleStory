#pragma once

#include "Component/ColliderBox2D.h"

class CDragCollider :
    public CColliderBox2D
{
    friend class CGameObject;

protected:
    CDragCollider();
    CDragCollider(const CDragCollider& com);
    virtual ~CDragCollider();

public:
    void SetColliderColor(const Vector4& Color);
    
public:
    virtual void Render();

public:
    // Init부터 Start, Collision 등 모든 함수는 부모인 CColliderBox2D에만 정의되어 있으니 그걸 호출하게 될것
    // 단순히 TypeID만 CColliderBox2D와 구별되는 클래스
    virtual CDragCollider* Clone();
};

