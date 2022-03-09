
#include "LopeTileObject.h"
#include "../Component/DragCollider.h"
#include "Component/TileMapComponent.h"

CLopeTileObject::CLopeTileObject()
{
	SetTypeID<CLopeTileObject>();
}

CLopeTileObject::CLopeTileObject(const CLopeTileObject& obj) :
	CGameObject(obj)
{

}

CLopeTileObject::~CLopeTileObject()
{
}

void CLopeTileObject::Start()
{
	CGameObject::Start();

	/*CTileMapComponent* TileComponent = FindComponentFromType<CTileMapComponent>();

	if (TileComponent)
		TileComponent->EnableClientMode(true);*/
}

bool CLopeTileObject::Init()
{

	return true;
}

void CLopeTileObject::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CLopeTileObject::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CLopeTileObject* CLopeTileObject::Clone()
{
	return new CLopeTileObject(*this);
}

void CLopeTileObject::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CLopeTileObject::Load(FILE* File)
{
	CGameObject::Load(File);

}

void CLopeTileObject::DeleteAllDragCollider()
{
	if (m_RootComponent->GetTypeID() == typeid(CDragCollider).hash_code())
	{
		m_RootComponent->Destroy();
		return;
	}
}



