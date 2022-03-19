#pragma once

#include "GameObject/GameObject.h"
#include "Component/SpriteComponent.h"

class CClapWarning :
    public CGameObject
{
    friend class CScene;

protected:
    CClapWarning();
    CClapWarning(const CClapWarning& obj);
    virtual ~CClapWarning();

private:
    CSharedPtr<CSpriteComponent>    m_Sprite;
    bool                            m_OnBright;
    class CZakumBody* m_WarningPointOwner;

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CClapWarning* Clone();

public:
    void SetWarningPointOwner(class CZakumBody* Zakum);

public:
    void SetClapWarningSpriteTexture(const std::string& Name, const TCHAR* FileName);

};

