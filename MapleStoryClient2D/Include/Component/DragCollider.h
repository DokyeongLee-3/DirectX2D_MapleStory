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



public:
    // Init���� Collision �� ���� ���� ��� �Լ��� �θ��� CColliderBox2D���� ���ǵǾ� ������ �װ� ȣ���ϰ� �ɰ�
    // �ܼ��� TypeID�� CColliderBox2D�� �����Ǵ� Ŭ����
    virtual void Start();
    CDragCollider* Clone();
    
};

