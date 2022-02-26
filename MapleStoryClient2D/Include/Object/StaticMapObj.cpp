
#include "StaticMapObj.h"
#include "Input.h"
#include "Component/ColliderComponent.h"
#include "Player2D.h"
#include "Engine.h"

CStaticMapObj::CStaticMapObj() :
	m_Sprite(nullptr),
	m_CollisionID(-1)
{
	SetTypeID<CStaticMapObj>();
}

CStaticMapObj::CStaticMapObj(const CStaticMapObj& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("StaticMapObjSprite");
}

CStaticMapObj::~CStaticMapObj()
{
}

void CStaticMapObj::Start()
{
	CGameObject::Start();
}

bool CStaticMapObj::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("StaticMapObjSprite");

	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(500.f, 300.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);


	return true;
}

void CStaticMapObj::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStaticMapObj::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CStaticMapObj* CStaticMapObj::Clone()
{
	return new CStaticMapObj(*this);
}

void CStaticMapObj::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CStaticMapObj::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("StaticMapObjSprite");
	m_Sprite->SetTransparency(true);
}

void CStaticMapObj::CollisionBeginCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		bool TileCollisionEnable = DestObj->GetTileCollisionEnable();

		// 점프 힘이 중력보다 아직 쎄서 위로 올라가고 있을 경우
		if (((CPlayer2D*)DestObj)->GetOnJump())
		{
			if (DestObj->GetGravityAccTime() * DestObj->GetGravityFactor() < ((CPlayer2D*)DestObj)->GetJumpForce() * CEngine::GetInst()->GetDeltaTime())
				return;
		}

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


			// CurrentFrameMove.y가 0보다 작아야하는 이유는 오브젝트가 자유낙하할때만 타일같은 밟을 수 있는 충돌체와 충돌처리 해야하기 때문
			if (DestBottomPos <= SrcColliderPos + SrcYScale / 2.f && DestBottomPos > SrcColliderPos &&
				(HalfHeight + PlayerHalfHeight) - (DestColliderPos - SrcColliderPos) < 6.f)
			{
				DestObj->SetTileCollisionEnable(true);
				DestObj->SetTileCollisionPos(DestObj->GetWorldPos());
				DestObj->SetGravity(false);
				DestObj->SetGravityAccTime(0.f);
				((CPlayer2D*)DestObj)->SetOnJump(false);
				((CPlayer2D*)DestObj)->GetRootSpriteComponent()->ChangeAnimation("IdleLeft");
				((CPlayer2D*)DestObj)->SetDir(PlayerDir::None);

				if(!((CPlayer2D*)DestObj)->CheckCollisionID(m_CollisionID))
					((CPlayer2D*)DestObj)->AddCollisionID(m_CollisionID);


			}

		}
	}
}

void CStaticMapObj::CollisionEndCallback(const CollisionResult& Result)
{
	CGameObject* DestObj = Result.Dest->GetGameObject();

	if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
	{
		if (((CPlayer2D*)DestObj)->CheckCollisionID(m_CollisionID))
		{
			if (DestObj->GetTypeID() == typeid(CPlayer2D).hash_code())
			{
				DestObj->SetGravity(true);
				((CPlayer2D*)DestObj)->SetTileCollisionEnable(false);
				((CPlayer2D*)DestObj)->EraseCollisionID(m_CollisionID);
			}
		}
	}
}



