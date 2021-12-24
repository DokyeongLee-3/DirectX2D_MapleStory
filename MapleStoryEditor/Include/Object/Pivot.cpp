
#include "Pivot.h"
#include "Component/ColorComponent.h"
#include "Scene/Scene.h"

CPivot::CPivot()
{
}

CPivot::CPivot(const CPivot& obj)	:
	CGameObject(obj)
{
}

CPivot::~CPivot()
{
}

bool CPivot::Init()
{
	m_ColorComponent = CreateComponent<CColorComponent>("PivotComponent");

	SetRootComponent(m_ColorComponent);

	m_ColorComponent->SetRelativeScale(100.f, 100.f, 1.f);

	m_ColorComponent->SetLayerName("DragLayer");

	return true;
}

void CPivot::Update(float DeltaTime)
{
	CGameObject::Update(DeltaTime);
}

void CPivot::PostUpdate(float DeltaTime)
{
	CGameObject::PostUpdate(DeltaTime);
}

CPivot* CPivot::Clone()
{
	return new CPivot(*this);
}
