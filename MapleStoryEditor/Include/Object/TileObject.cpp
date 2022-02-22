
#include "TileObject.h"
#include "../Component/DragCollider.h"


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

void CTileObject::DeleteAllDragCollider()
{
	if (m_RootComponent->GetTypeID() == typeid(CDragCollider).hash_code())
	{
		m_RootComponent->Destroy();
		return;
	}
}



