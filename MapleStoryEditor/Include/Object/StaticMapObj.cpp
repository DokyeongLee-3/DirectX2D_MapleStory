
#include "StaticMapObj.h"
#include "../Component/DragCollider.h"


CStaticMapObj::CStaticMapObj() :
	m_Sprite(nullptr)
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
	m_DragCollider = CreateComponent<CDragCollider>("DragCollider");


	m_DragCollider->SetCollisionProfile("DragCollider");

	SetRootComponent(m_Sprite);
	m_Sprite->AddChild(m_DragCollider);



	m_DragCollider->SetWorldScale(100.f, 100.f, 1.f);

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

void CStaticMapObj::DeleteAllDragCollider()
{
	if (m_RootComponent->GetTypeID() == typeid(CDragCollider).hash_code())
	{
		m_RootComponent->Destroy();
		return;
	}
}



