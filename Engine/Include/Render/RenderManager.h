#pragma once

#include "../GameInfo.h"
#include "../Component/SceneComponent.h"

struct RenderLayer
{
	std::string Name;
	// Priority가 높은 LayerList에 있는 Component가
	// 늦게 출력되도록 한다
	int LayerPriority;
	std::vector<class CSceneComponent*> RenderList;
	int RenderCount;
	float LayerZUpperLimit;
	float LayerZLowerLimit;

	RenderLayer()
	{
		LayerPriority = 0;
		RenderCount = 0;
		RenderList.resize(500);
		LayerZUpperLimit = 0;
		LayerZLowerLimit = 0;
	}

	static bool SortSceneComponent(CSceneComponent* Src, CSceneComponent* Dest)
	{
		return Src->GetZOrder() < Dest->GetZOrder();
	}
};

class CRenderManager
{
private:
	class CRenderStateManager* m_RenderStateManager;

private:
	const std::list<CSharedPtr<class CGameObject>>* m_ObjectList;
	// 오브젝트 내에 여러개의 SceneComponent의 출력 순서나
	// 다른 오브젝트내의 SceneComponent간의 출력 순서를
	// 여기서 정렬해준다 
	std::vector<RenderLayer*>	m_RenderLayerList;
	class CStandard2DConstantBuffer* m_Standard2DCBuffer;
	class CRenderState* m_DepthDisable;
	class CRenderState* m_AlphaBlend;
	float m_FadeAmount;
	bool m_StartFadeIn;
	bool m_StartFadeOut;

	float m_FadeInLimit;
	float m_FadeOutLimit;
	float m_FadeInSpeed;
	float m_FadeOutSpeed;

public:
	float GetFadeInSpeed()	const
	{
		return m_FadeInSpeed;
	}

	float GetFadeOutSpeed()	const
	{
		return m_FadeOutSpeed;
	}

	void SetFadeInSpeed(float Speed)
	{
		m_FadeInSpeed = Speed;
	}

	void SetFadeOutSpeed(float Speed)
	{
		m_FadeOutSpeed = Speed;
	}

	bool GetStartFadeIn()	const
	{
		return m_StartFadeIn;
	}

	bool GetStartFadeOut()	const
	{
		return m_StartFadeOut;
	}

	void SetStartFadeIn(bool FadeInStart)
	{
		m_StartFadeIn = FadeInStart;
	}

	void SetStartFadeOut(bool FadeOutStart)
	{
		m_StartFadeOut = FadeOutStart;
	}

	void SetFadeInLimit(float Limit)
	{
		m_FadeInLimit = Limit;
	}

	void SetFadeOutLimit(float Limit)
	{
		m_FadeOutLimit = Limit;
	}

	float GetFadeInLimit()	const
	{
		return m_FadeInLimit;
	}

	float GetFadeOutLimit()	const
	{
		return m_FadeOutLimit;
	}

public:
	float GetLayerLowerBoundZOrder(std::string LayerName);
	float GetLayerUpperBoundZOrder(std::string LayerName);

public:
	class CStandard2DConstantBuffer* GetStandard2DCBuffer()	const
	{
		return m_Standard2DCBuffer;
	}

public:
	void SetObjectList(const std::list<CSharedPtr<class CGameObject>>* List)
	{
		m_ObjectList = List;
	}

	void AddRenderList(class CSceneComponent* Component);
	void CreateLayer(const std::string& Name, int Priority);
	void SetLayerPriority(const std::string& Name, int Priority);
	void SetFadeAmount(float FadeAmount);

public:
	float GetFadeAmount()	const
	{
		return m_FadeAmount;
	}

public:
	bool Init();
	void Render();

public:
	void SetAlphaBlendState();
	void ResetAlphaBlendState();

	// Render State
public:
	void SetBlendFactor(const std::string& Name, float r, float g, float b, float a);
	void AddBlendInfo(const std::string& Name, bool BlendEnable = true, D3D11_BLEND SrcBlend = D3D11_BLEND_SRC_ALPHA,
		D3D11_BLEND DestBlend = D3D11_BLEND_INV_SRC_ALPHA, D3D11_BLEND_OP BlendOp = D3D11_BLEND_OP_ADD,
		D3D11_BLEND SrcBlendAlpha = D3D11_BLEND_ONE, D3D11_BLEND DestBlendAlpha = D3D11_BLEND_ZERO,
		D3D11_BLEND_OP BlendOpAlpha = D3D11_BLEND_OP_ADD,
		UINT8 RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL);
	bool CreateBlendState(const std::string& Name, bool AlphaToCoverageEnable, bool IndependentBlendEnable);

public:
	class CRenderState* FindRenderState(const std::string& Name);

private:
	static bool Sortlayer(RenderLayer* Src, RenderLayer* Dest);


public:
	void FadeIn(float DeltaTime);
	void FadeOut(float DeltaTime);

	DECLARE_SINGLE(CRenderManager)
};

