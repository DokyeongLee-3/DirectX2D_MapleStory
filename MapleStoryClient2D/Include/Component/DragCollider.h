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

protected:
    virtual void Update(float DeltaTime);


public:
    // Init부터 Collision 등 여기 없는 모든 함수는 부모인 CColliderBox2D에만 정의되어 있으니 그걸 호출하게 될것
    // 단순히 TypeID만 CColliderBox2D와 구별되는 클래스
    virtual void Start();
    CDragCollider* Clone();
    
};

