#pragma once

#include "SceneComponent.h"
#include "../Widget/WidgetWindow.h"
#include "../Scene/Scene.h"

class CWidgetComponent :
    public CSceneComponent
{
    friend class CGameObject;

protected:
    CWidgetComponent();
    CWidgetComponent(const CWidgetComponent& com);
    virtual ~CWidgetComponent();

protected:
    CSharedPtr<CWidgetWindow>   m_WidgetWindow;
    WidgetComponent_Space       m_Space;
    // 알파채널 사용할 것인지 안할것인지
    bool                        m_SetAlphaBlendState;

public:
    void UseAlphaBlend(bool Use)
    {
        m_SetAlphaBlendState = Use;
    }

    bool GetAlphablendState()   const
    {
        return m_SetAlphaBlendState;
    }

public:
    void SetWidgetSpace(WidgetComponent_Space Space)
    {
        m_Space = Space;

        if (m_Space == WidgetComponent_Space::Screen)
        {
            SetInheritRotX(false);
            SetInheritRotY(false);
            SetInheritRotZ(false);
            SetLayerName("ScreenWidgetComponent");
        }

        else
        {
            SetInheritRotX(true);
            SetInheritRotY(true);
            SetInheritRotZ(true);
            SetLayerName("MapObjBack");
        }
    }

    WidgetComponent_Space GetWidgetSpace()  const
    {
        return m_Space;
    }

    void SetWidgetWindow(CWidgetWindow* Window)
    {
        if (m_WidgetWindow)
            m_WidgetWindow->m_OwnerComponent = nullptr;

        m_WidgetWindow = Window;

        m_WidgetWindow->m_OwnerComponent = this;
    }

    CWidgetWindow* GetWidgetWindow()    const
    {
        return m_WidgetWindow;
    }

public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual void PrevRender();
    virtual void Render();
    virtual void PostRender();
    virtual CWidgetComponent* Clone();
    virtual void Save(FILE* File);
    virtual void Load(FILE* File);

public:
    template <typename T>
    T* CreateWidgetWindow(const std::string& Name)
    {
        T* Window = new T;

        Window->SetName(Name);
        Window->m_OwnerComponent = this;

        if (m_Scene->GetViewport())
            Window->m_Viewport = m_Scene->GetViewport();

        
        if (!Window->Init())
        {
            SAFE_RELEASE(Window);
            return nullptr;
        }

        m_WidgetWindow = Window;

        return Window;
    }
};

