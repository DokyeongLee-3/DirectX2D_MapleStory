
#include "TileObject.h"
#include "../Component/DragCollider.h"
#include "Player2D.h"
#include "Component/TileMapComponent.h"
#include "LopeTileObject.h"

CTileObject::CTileObject()	:
	m_BottomMostFloor(false)
{
	SetTypeID<CTileObject>();
}

CTileObject::CTileObject(const CTileObject& obj) :
	CGameObject(obj)
{
}

CTileObject::~CTileObject()
{
}

void CTileObject::Start()
{
	CGameObject::Start();

	if (m_Name.find("Floor") != std::string::npos)
		m_BottomMostFloor = true;

	m_Floor = FindComponentFromType<CColliderBox2D>();

	if (m_Floor)
	{
		m_Floor->AddCollisionCallback(Collision_State::Begin, this, &CTileObject::CollisionBeginCallback);
		m_Floor->AddCollisionCallback(Collision_State::End, this, &CTileObject::CollisionEndCallback);
	}

	CTileMapComponent* TileComponent = FindComponentFromType<CTileMapComponent>();

	if (TileComponent)
		TileComponent->EnableClientMode(true);
}

bool CTileObject::Init()
{

	return true;
}

void CTileObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CTileObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CTileObject* CTileObject::Clone()
{
	return new CTileObject(*this);
}

void CTileObject::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CTileObject::Load(FILE* File)
{
	CGameObject::Load(File);
}

void CTileObject::CollisionBeginCallback(const CollisionResult& Result)
{
	if (!Result.Dest)
		return;

	CGameObject* DestObj = Result.Dest->GetGameObject();

	bool TileCollisionEnable = DestObj->GetTileCollisionEnable();

	if (!TileCollisionEnable)
	{
		float DestYScale = Result.Dest->GetWorldScale().y;
		float SrcYScale = Result.Src->GetWorldScale().y;
		// Src 충돌체의 현재 WorldPos
		float SrcColliderPos = Result.Src->GetWorldPos().y + Result.Src->GetOffset().y;
		// Dest 충돌체의 현재 WorldPos
		float DestColliderPos = Result.Dest->GetWorldPos().y + Result.Dest->GetOffset().y;
		// Src 충돌체의 높이의 절반
		float HalfHeight = SrcYScale / 2.f;
		// Dest 충돌체의 높이의 절반
		float PlayerHalfHeight = DestYScale / 2.f;

		// Player의 발밑 좌표 -> Player는 Pivot이 (0.5, 0.5)이다
		float DestBottomPos = DestColliderPos - DestYScale / 2.f;


		if (DestBottomPos <= SrcColliderPos + SrcYScale / 2.f && DestBottomPos > SrcColliderPos &&
			(HalfHeight + PlayerHalfHeight) - (DestColliderPos - SrcColliderPos) < 6.f)
		{
			DestObj->SetTileCollisionEnable(true);
			DestObj->SetTileCollisionPos(DestObj->GetWorldPos());
			DestObj->SetGravity(false);
			DestObj->SetGravityAccTime(0.f);

			if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
			{
				CPlayer2D* PlayerObj = ((CPlayer2D*)DestObj);

				PlayerObj->SetOnJump(false);
				PlayerObj->GetRootSpriteComponent()->ChangeAnimation("IdleLeft");

				if (PlayerObj->GetOnKnockBack())
				{
					PlayerObj->SetOnKnockBack(false);
					PlayerObj->SetOnKnockBackAccTime(0.f);
				}
			}
		}

	}
}

void CTileObject::CollisionEndCallback(const CollisionResult& Result)
{
	if (!Result.Dest)
		return;

	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj) 
	{
		if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
		{
			CPlayer2D* PlayerObj = (CPlayer2D*)DestObj;
			if (!(PlayerObj->GetPlayerBody()->CheckPrevCollisionGameObjectType(typeid(CLopeTileObject).hash_code())))
			{
				// 플레이어가 로프에서 메달렸다가 옆으로 뛰어 내리다가 머리가 타일 하단부랑 부딪혔다가 떨어질때는 예외로 둔다
				if (PlayerObj->GetIsLopeJump() && PlayerObj->GetCurrentFrameMove().y < 0.f)
				{
					return;
				}

				DestObj->SetGravity(true);
				DestObj->SetTileCollisionEnable(false);
			}
		}
	}
}



