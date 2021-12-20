
#include "BulletTornaido.h"
#include "Component/SpriteComponent.h"

CBulletTornaido::CBulletTornaido()
{
}

CBulletTornaido::CBulletTornaido(const CBulletTornaido& obj) :
	CGameObject(obj)
{
	m_Root = (CSceneComponent*)FindComponent("Root");
	m_Root1 = (CSceneComponent*)FindComponent("Root1");
	m_Sprite = (CSpriteComponent*)FindComponent("BulletSprite");
}

CBulletTornaido::~CBulletTornaido()
{
}

bool CBulletTornaido::Init()
{
	// m_Root는 이동하는 용도
	m_Root = CreateComponent<CSceneComponent>("Root");
	// m_Root1은 회전하는 용도
	m_Root1 = CreateComponent<CSceneComponent>("Root1");
	// m_Sprite는 m_Root 주위를 공전하면서 m_Root1이 이동하는 만큼
	// 따라서 이동함
	m_Sprite = CreateComponent<CSpriteComponent>("BulletSprite");

	SetRootComponent(m_Root);

	m_Root->AddChild(m_Root1);
	m_Root1->AddChild(m_Sprite);

	m_Sprite->SetRelativeScale(50.f, 50.f, 1.f);
	m_Sprite->SetRelativePos(50.f, 0.f, 0.f);
	m_Sprite->SetPivot(0.5f, 0.5f, 0.f);
	m_Sprite->SetInheritRotZ(true);

	return true;
}

void CBulletTornaido::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);

	AddRelativePos(GetWorldAxis(AXIS_Y) * 500.f * DeltaTime);

	m_Root1->AddRelativeRotationZ(720.f * DeltaTime);
}

void CBulletTornaido::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CBulletTornaido* CBulletTornaido::Clone()
{
	return new CBulletTornaido(*this);
}
