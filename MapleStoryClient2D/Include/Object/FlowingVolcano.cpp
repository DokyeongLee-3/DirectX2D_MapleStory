
#include "FlowingVolcano.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CFlowingVolcano::CFlowingVolcano()
{
	SetTypeID<CFlowingVolcano>();
	m_Count = 33;
}

CFlowingVolcano::CFlowingVolcano(const CFlowingVolcano& obj) :
	CGameObject(obj)
{
	m_MovingTileMap = (CMovingTileMapComponent*)FindComponent("FlowingVolcanoSprite");
}

CFlowingVolcano::~CFlowingVolcano()
{
}

void CFlowingVolcano::Start()
{
	CGameObject::Start();
}

bool CFlowingVolcano::Init()
{
	m_MovingTileMap = CreateComponent<CMovingTileMapComponent>("FlowingVolcanoSprite");
	m_MovingTileMap->SetLayerName("CoveringMapObj");

	SetRootComponent(m_MovingTileMap);
	m_MovingTileMap->SetZOrder(3);

	m_MovingTileMap->SetWorldScale(100.f, 100.f, 1.f);
	m_MovingTileMap->SetSpeed(Vector2(-20.f, 0.f));
	//m_Sprite->SetPivot(0.5f, 0.5f, 0.f);

	//m_Sprite->SetLayerName("MapObjBack");

	//CAnimationSequence2DInstance* Anim = m_Sprite->GetAnimationInstance();

	//Anim->AddAnimation(TEXT("FlowingVolcano.sqc"), ANIMATION_PATH, "FlowingVolcano", true, 0.6f);

	m_MovingTileMap->CreateMovingTile(m_Count, Vector2(88.f, 96.f));
	m_MovingTileMap->SetRepeatPos(Vector2((m_Count - 1) * 88.f, 35.f));

	CMaterial* Material = m_Scene->GetResource()->FindMaterial("Volcano");

	m_MovingTileMap->SetMaterial(Material);

	//m_MovingTileMap->SetDefaultFrame(0.f, 0.f, 88.f, 96.f);

	for (int i = 1; i <= m_Count; ++i)
	{
		if (i % 3 == 1)
		{
			m_MovingTileMap->SetFrame(i-1, Vector2(0.f, 0.f), Vector2(88.f, 92.f));
		}

		else if(i % 3 == 2)
		{
			m_MovingTileMap->SetFrame(i-1, Vector2(88.f, 0.f), Vector2(176.f, 96.f));
		}

		else
		{
			m_MovingTileMap->SetFrame(i-1, Vector2(176.f, 0.f), Vector2(264.f, 96.f));
		}
	}

	return true;
}

void CFlowingVolcano::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CFlowingVolcano::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CFlowingVolcano* CFlowingVolcano::Clone()
{
	return new CFlowingVolcano(*this);
}


