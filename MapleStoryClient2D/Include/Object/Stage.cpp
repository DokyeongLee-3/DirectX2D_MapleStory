
#include "Stage.h"


CStage::CStage() :
	m_Sprite(nullptr)
{
	SetTypeID<CStage>();
}

CStage::CStage(const CStage& obj) :
	CGameObject(obj)
{
	m_Sprite = (CSpriteComponent*)FindComponent("StageSprite");
}

CStage::~CStage()
{
}

void CStage::Start()
{
	CGameObject::Start();
}

bool CStage::Init()
{
	m_Sprite = CreateComponent<CSpriteComponent>("StageSprite");


	SetRootComponent(m_Sprite);

	m_Sprite->SetTransparency(true);
	m_Sprite->SetWorldScale(100.f, 100.f, 1.f);
	m_Sprite->SetRelativePos(0.f, 0.f, 0.f);
	m_Sprite->SetPivot(0.f, 0.f, 0.f);


	return true;
}

void CStage::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CStage::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CStage* CStage::Clone()
{
	return new CStage(*this);
}

void CStage::Save(FILE* File)
{
	CGameObject::Save(File);
}

void CStage::Load(FILE* File)
{
	CGameObject::Load(File);

	m_Sprite = (CSpriteComponent*)FindComponent("StageSprite");
}



