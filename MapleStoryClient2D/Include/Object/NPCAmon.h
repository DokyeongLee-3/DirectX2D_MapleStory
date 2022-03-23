#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/WidgetComponent.h"

class CNPCAmon :
    public CGameObject
{
    friend class CScene;

protected:
    CNPCAmon();
    CNPCAmon(const CNPCAmon& obj);
    virtual ~CNPCAmon();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    CSharedPtr<CColliderBox2D>      m_Body;
    CSharedPtr<CWidgetComponent>    m_NameWidget;
    class CAmonDialog* m_Dialog;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CNPCAmon* Clone();

public:
    void CreateAmonDialog();
    void GoToWayToZakumScene();

public:
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

};

