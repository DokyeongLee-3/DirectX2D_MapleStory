
#include "ParticleSystemWindow.h"
#include "IMGUIIMage.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "Input.h"

CParticleSystemWindow::CParticleSystemWindow()
{
	
}

CParticleSystemWindow::~CParticleSystemWindow()
{
}

bool CParticleSystemWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("", 500.f, 20.f);
	Label->SetColor(0, 0, 0, 0);

	m_RainMaterialImage = AddWidget<CIMGUIImage>("RainMaterialImage", 150.f, 150.f);
	m_RainMaterialImage->SetTexture("RainImage", TEXT("Rain.png"));
	m_RainMaterialImage->SetHoverCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverRainMaterial);
	m_RainMaterialImage->SetHoverEndCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverEndRainMaterial);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(170.f);

	m_DustMaterialImage = AddWidget<CIMGUIImage>("DustMaterialImage", 150.f, 150.f);
	m_DustMaterialImage->SetTexture("DustImage", TEXT("Dust.png"));
	m_DustMaterialImage->SetHoverCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverDustMaterial);
	m_DustMaterialImage->SetHoverEndCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverEndDustMaterial);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SmokeMaterialImage = AddWidget<CIMGUIImage>("SmokeMaterialImage", 150.f, 150.f);
	m_SmokeMaterialImage->SetTexture("SmokeImage", TEXT("Smoke.png"));
	m_SmokeMaterialImage->SetHoverCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverSmokeMaterial);
	m_SmokeMaterialImage->SetHoverEndCallback<CParticleSystemWindow>(this, &CParticleSystemWindow::HoverEndSmokeMaterial);

	return true;
}

void CParticleSystemWindow::Update(float DeltaTime)
{

	CIMGUIWindow::Update(DeltaTime);
}

void CParticleSystemWindow::HoverRainMaterial()
{
	if (m_SelectMaterial == Select_ParticleMaterial::Rain)
	{
		m_RainOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_RainMaterialImage->SetBorderColor(255, 0, 0);
	}

	else
	{
		m_RainOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_RainMaterialImage->SetBorderColor(255, 255, 0);
	}

	if (CInput::GetInst()->GetMouseLButtonClick())
	{
		m_SelectMaterial = Select_ParticleMaterial::Rain;
		m_RainMaterialImage->SetBorderColor(255, 0, 0);
		m_DustMaterialImage->SetBorderColor(0, 0, 0);
		m_SmokeMaterialImage->SetBorderColor(0, 0, 0);
	}
}

void CParticleSystemWindow::HoverDustMaterial()
{
	if (m_SelectMaterial == Select_ParticleMaterial::Dust)
	{
		m_DustOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_DustMaterialImage->SetBorderColor(255, 0, 0);
	}

	else
	{
		m_DustOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_DustMaterialImage->SetBorderColor(255, 255, 0);
	}

	if (CInput::GetInst()->GetMouseLButtonClick())
	{
		m_SelectMaterial = Select_ParticleMaterial::Dust;
		m_RainMaterialImage->SetBorderColor(0, 0, 0);
		m_DustMaterialImage->SetBorderColor(255, 0, 0);
		m_SmokeMaterialImage->SetBorderColor(0, 0, 0);
	}
}

void CParticleSystemWindow::HoverSmokeMaterial()
{
	if (m_SelectMaterial == Select_ParticleMaterial::Smoke)
	{
		m_SmokeOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_SmokeMaterialImage->SetBorderColor(255, 0, 0);
	}

	else
	{
		m_SmokeOriginBorderColor = ImVec4(0.f, 0.f, 0.f, 0.f);
		m_SmokeMaterialImage->SetBorderColor(255, 255, 0);
	}

	if (CInput::GetInst()->GetMouseLButtonClick())
	{
		m_SelectMaterial = Select_ParticleMaterial::Smoke;
		m_RainMaterialImage->SetBorderColor(0, 0, 0);
		m_DustMaterialImage->SetBorderColor(0, 0, 0);
		m_SmokeMaterialImage->SetBorderColor(255, 0, 0);
	}
}

void CParticleSystemWindow::HoverEndRainMaterial()
{
	if(m_SelectMaterial != Select_ParticleMaterial::Rain)
		m_RainMaterialImage->SetBorderColor((unsigned char)m_RainOriginBorderColor.x * 255, (unsigned char)m_RainOriginBorderColor.y * 255, (unsigned char)m_RainOriginBorderColor.z * 255);
}

void CParticleSystemWindow::HoverEndDustMaterial()
{
	if (m_SelectMaterial != Select_ParticleMaterial::Dust)
		m_DustMaterialImage->SetBorderColor((unsigned char)m_RainOriginBorderColor.x * 255, (unsigned char)m_RainOriginBorderColor.y * 255, (unsigned char)m_RainOriginBorderColor.z * 255);
}

void CParticleSystemWindow::HoverEndSmokeMaterial()
{
	if (m_SelectMaterial != Select_ParticleMaterial::Smoke)
		m_SmokeMaterialImage->SetBorderColor((unsigned char)m_RainOriginBorderColor.x * 255, (unsigned char)m_RainOriginBorderColor.y * 255, (unsigned char)m_RainOriginBorderColor.z * 255);
}
