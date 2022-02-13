#pragma once

#include "GameObject/GameObject.h"
#include "Component/MovingTileMapComponent.h"

class CFlowingVolcano :
    public CGameObject
{
    friend class CScene;

protected:
    CFlowingVolcano();
    CFlowingVolcano(const CFlowingVolcano& obj);
    virtual ~CFlowingVolcano();

private:
    CSharedPtr<CMovingTileMapComponent>     m_MovingTileMap;
    int                                     m_Count;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CFlowingVolcano* Clone();
};

