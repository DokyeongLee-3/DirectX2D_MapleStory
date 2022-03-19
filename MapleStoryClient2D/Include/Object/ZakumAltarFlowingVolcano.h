#pragma once

#include "GameObject/GameObject.h"
#include "Component/MovingTileMapComponent.h"

class CZakumAltarFlowingVolcano :
    public CGameObject
{
    friend class CScene;

protected:
    CZakumAltarFlowingVolcano();
    CZakumAltarFlowingVolcano(const CZakumAltarFlowingVolcano& obj);
    virtual ~CZakumAltarFlowingVolcano();

private:
    CSharedPtr<CMovingTileMapComponent>     m_MovingTileMap;
    int                                     m_Count;


public:
    virtual void Start();
    virtual bool Init();
    virtual void Update(float DeltaTime);
    virtual void PostUpdate(float DeltaTime);
    virtual CZakumAltarFlowingVolcano* Clone();
};

