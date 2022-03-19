
#include "ZakumAltarFlowingVolcano.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CZakumAltarFlowingVolcano::CZakumAltarFlowingVolcano()
{
	m_Count = 6;
	SetTypeID<CZakumAltarFlowingVolcano>();
}

CZakumAltarFlowingVolcano::CZakumAltarFlowingVolcano(const CZakumAltarFlowingVolcano& obj)	:
	CGameObject(obj)
{
	m_MovingTileMap = (CMovingTileMapComponent*)FindComponent("ZakumAltarFlowingVolcanoSprite");
}

CZakumAltarFlowingVolcano::~CZakumAltarFlowingVolcano()
{
}

void CZakumAltarFlowingVolcano::Start()
{
	CGameObject::Start();
}

bool CZakumAltarFlowingVolcano::Init()
{
	m_MovingTileMap = CreateComponent<CMovingTileMapComponent>("ZakumAltarFlowingVolcanoSprite");
	m_MovingTileMap->SetLayerName("CoveringMapObj");

	SetRootComponent(m_MovingTileMap);
	m_MovingTileMap->SetZOrder(3);

	//m_MovingTileMap->SetWorldScale(100.f, 100.f, 1.f);
	m_MovingTileMap->SetSpeed(Vector2(-20.f, 0.f));

	m_MovingTileMap->CreateMovingTile(m_Count, Vector2(330.f, 115.f));
	m_MovingTileMap->SetRepeatPos(Vector2((m_Count - 1) * 330.f, 30.f));

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("ZakumAltarFlowingVolcano");

	m_MovingTileMap->SetMaterial(Material);
	m_MovingTileMap->SetTransparency(true);

	for (int i = 0; i < m_Count; ++i)
	{
		m_MovingTileMap->SetFrame(i, Vector2(0.f, 0.f), Vector2(329.f, 114.f));

		Vector2 TilePos = m_MovingTileMap->GetTilePos(i);
		m_MovingTileMap->SetTilePos(i, Vector2(TilePos.x, 10.f));
	}


	return true;
}

void CZakumAltarFlowingVolcano::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CZakumAltarFlowingVolcano::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CZakumAltarFlowingVolcano* CZakumAltarFlowingVolcano::Clone()
{
	return new CZakumAltarFlowingVolcano(*this);
}
