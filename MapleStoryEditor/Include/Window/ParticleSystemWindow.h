#pragma once

#include "IMGUIWindow.h"
#include "../EditorInfo.h"

class CParticleSystemWindow :
    public CIMGUIWindow
{
public:
	CParticleSystemWindow();
	virtual ~CParticleSystemWindow();

public:
	class CIMGUIImage* m_RainMaterialImage;
	class CIMGUIImage* m_DustMaterialImage;
	class CIMGUIImage* m_SmokeMaterialImage;

	ImVec4 m_RainOriginBorderColor;
	ImVec4 m_DustOriginBorderColor;
	ImVec4 m_SmokeOriginBorderColor;

	Select_ParticleMaterial m_SelectMaterial;

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

public:
	Select_ParticleMaterial	GetSelectParticle()	const
	{
		return m_SelectMaterial;
	}

public:
	void HoverRainMaterial();
	void HoverDustMaterial();
	void HoverSmokeMaterial();
	void HoverEndRainMaterial();
	void HoverEndDustMaterial();
	void HoverEndSmokeMaterial();
};

