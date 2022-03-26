
#include "NPCName.h"
#include "Component/WidgetComponent.h"
#include "../Object/NPCAdobis.h"
#include "../Object/NPCAmon.h"

CNPCName::CNPCName()
{
}

CNPCName::CNPCName(const CNPCName& window)	:
	CWidgetWindow(window)
{
}

CNPCName::~CNPCName()
{
}

void CNPCName::Start()
{
	CWidgetWindow::Start();
}

bool CNPCName::Init()
{
	if (!CWidgetWindow::Init())
		return false;

	SetSize(40.f, 65.f);
	SetMouseCollisionEnable(true);

	m_NPCName = CreateWidget<CText>("NPCName");
	m_NPCName->SetSize(40.f, 15.f);
	m_NPCName->SetPos(0.f, 0.f);
	m_NPCName->SetFontSize(10.f);
	m_NPCName->SetColor(1.f, 1.f, 0.f);
	m_NPCName->SetZOrder(1);
	m_NPCName->SetAlignH(TEXT_ALIGN_H::Center);
	m_NPCName->SetAlphaEnable(true);
	m_NPCName->SetFont("Default");

	m_DoubleClickFrame = CreateWidget<CImage>("NPCBlankFrame");
	m_DoubleClickFrame->SetPos(0.f, 10.f);
	m_DoubleClickFrame->SetSize(40.f, 65.f);
	m_DoubleClickFrame->SetTexture("NPCBlankFrame", TEXT("NPC/BlankFrame.png"));
	m_DoubleClickFrame->SetMouseCollisionEnable(true);
	m_DoubleClickFrame->SetDoubleClickCallback<CNPCName>(this, &CNPCName::OpenDialog);

	return true;
}

void CNPCName::Update(float DeltaTime)
{
	CWidgetWindow::Update(DeltaTime);
}

void CNPCName::PostUpdate(float DeltaTime)
{
	CWidgetWindow::PostUpdate(DeltaTime);
}

void CNPCName::Render()
{
	CWidgetWindow::Render();
}

void CNPCName::OpenDialog()
{
	if (m_OwnerComponent->GetGameObject()->GetTypeID() == typeid(CNPCAdobis).hash_code())
	{
		CNPCAdobis* Adobis = (CNPCAdobis*)m_OwnerComponent->GetGameObject();

		if (Adobis)
		{
			Adobis->CreateAdobisDialog();
		}
	}

	else if (m_OwnerComponent->GetGameObject()->GetTypeID() == typeid(CNPCAmon).hash_code())
	{
		CNPCAmon* Amon = (CNPCAmon*)m_OwnerComponent->GetGameObject();

		if (Amon)
		{
			Amon->CreateAmonDialog();
		}
	}
}

CNPCName* CNPCName::Clone()
{
	return new CNPCName(*this);
}
