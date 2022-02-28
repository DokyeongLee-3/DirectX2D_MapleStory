#pragma once

#include "Scene/SceneMode.h"
#include "../Widget/StartWidget.h"
#include "../Widget/CharacterSelectInfoWidget.h"
#include "../Object/Player2D.h"

class CStartScene :
    public CSceneMode
{
public:
    CStartScene();
    ~CStartScene();

private:
    CSharedPtr<CStartWidget> m_StartWidget;
    CSharedPtr<CCharacterSelectInfoWidget> m_CharacterSelectInfoWidget;
    CSharedPtr<CGameObject> m_Background;
    class CLoadingThread* m_LoadingThread;

    PlayerInfo m_Info;

public:
    const PlayerInfo& GetPlayerInfo() const
    {
        return m_Info;
    }

    const CStartWidget* GetStartWidget()  const
    {
        return m_StartWidget;
    }

public:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    void CreateNextScene();
};

