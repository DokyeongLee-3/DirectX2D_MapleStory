#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"

// ��ų ����Ʈ�� ������ ���� Test������ ȭ�鿡 ��� �뵵
class CPlayerSkillSet :
    public CGameObject
{
    friend class CScene;

protected:
    CPlayerSkillSet();
    CPlayerSkillSet(const CPlayerSkillSet& obj);
    virtual ~CPlayerSkillSet();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>       m_Body;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPlayerSkillSet* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);


private:
    void FlipAll(float DeltaTime);
    void NextAnimation(float DeltaTime);
};

