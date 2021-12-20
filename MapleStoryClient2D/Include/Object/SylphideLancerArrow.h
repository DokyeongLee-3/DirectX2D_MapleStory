#pragma once

#include "GameObject/GameObject.h"

class CSylphideLancerArrow :
    public CGameObject
{
    friend class CScene;

protected:
    CSylphideLancerArrow();
    CSylphideLancerArrow(const CSylphideLancerArrow& obj);
    virtual ~CSylphideLancerArrow();

private:
    CSharedPtr<class CSpriteComponent>    m_Sprite;
    float   m_Distance;
    float   m_Speed;

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CSylphideLancerArrow* Clone();

public:
    void ChangeRootSpriteAnimation(const std::string& Name);
};