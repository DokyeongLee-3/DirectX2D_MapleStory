#pragma once

#include "GameObject/GameObject.h"

class CPivot :
    public CGameObject
{
    friend class CScene;

protected:
    CPivot();
    CPivot(const CPivot& obj);
    virtual ~CPivot();

private:
    CSharedPtr<class CColorComponent>    m_ColorComponent;

    class CColorComponent* GetColorComponent()  const
    {
        return m_ColorComponent;
    }


public:
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CPivot* Clone();
};

