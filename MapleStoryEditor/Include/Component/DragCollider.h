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
    // Init���� Start, Collision �� ��� �Լ��� �θ��� CColliderBox2D���� ���ǵǾ� ������ �װ� ȣ���ϰ� �ɰ�
    // �ܼ��� TypeID�� CColliderBox2D�� �����Ǵ� Ŭ����
    virtual CDragCollider* Clone();
};

