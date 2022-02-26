
#include "TileObject.h"
#include "../Component/DragCollider.h"
#include "Player2D.h"

CTileObject::CTileObject()
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

	/*m_Floor->AddCollisionCallback(Collision_State::Begin, this, &CTileObject::CollisionBeginCallback);
	m_Floor->AddCollisionCallback(Collision_State::Stay, this, &CTileObject::CollisionStayCallback);
	m_Floor->AddCollisionCallback(Collision_State::End, this, &CTileObject::CollisionEndCallback);*/
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
				((CPlayer2D*)DestObj)->SetOnJump(false);
				((CPlayer2D*)DestObj)->GetRootSpriteComponent()->ChangeAnimation("IdleLeft");
			}
		}

	}
}

//void CTileObject::CollisionStayCallback(const CollisionResult& Result)
//{
//	CGameObject* Dest = Result.Dest->GetGameObject();
//	Vector3 DestPos = Dest->GetWorldPos();
//	Vector3 DestTileCollisionPos = Dest->GetTileCollisionPos();
//	Dest->SetWorldPos(DestPos.x, DestTileCollisionPos.y, DestPos.z);
//	Dest->SetGravity(false);
//	Dest->SetGravityAccTime(0.f);
//}

void CTileObject::CollisionEndCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	DestObj->SetGravity(true);
	DestObj->SetTileCollisionEnable(false);
}



