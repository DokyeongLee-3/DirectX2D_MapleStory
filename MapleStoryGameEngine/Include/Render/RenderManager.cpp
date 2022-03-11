
#include "RenderManager.h"
#include "../GameObject/GameObject.h"
#include "RenderStateManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "RenderState.h"
#include "../Scene/SceneManager.h"
#include "../Scene/Scene.h"
#include "../Scene/Viewport.h"
#include "../Input.h"
#include "../Engine.h"

DEFINITION_SINGLE(CRenderManager)

CRenderManager::CRenderManager() :
	m_RenderStateManager(nullptr),
	m_Standard2DCBuffer(nullptr),
	m_FadeAmount(1.f),
	m_StartFadeIn(false),
	m_StartFadeOut(false),
	m_FadeInLimit(0.f),
	m_FadeOutLimit(1.f),
	m_FadeInSpeed(0.4f),
	m_FadeOutSpeed(0.35f)
{
}

CRenderManager::~CRenderManager()
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE((*iter));
	}

	m_RenderLayerList.clear();

	SAFE_DELETE(m_Standard2DCBuffer);
	SAFE_DELETE(m_RenderStateManager);
}

float CRenderManager::GetLayerLowerBoundZOrder(std::string LayerName)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == LayerName)
			return((*iter)->LayerZLowerLimit);
	}

	return 0;
}

float CRenderManager::GetLayerUpperBoundZOrder(std::string LayerName)
{
	auto iter = m_RenderLayerList.begin();
	auto iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == LayerName)
			return((*iter)->LayerZUpperLimit);
	}

	return 0;
}

void CRenderManager::AddRenderList(CSceneComponent* Component)
{
	RenderLayer* Layer = nullptr;

	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Component->GetLayerName())
		{
			Layer = *iter;
			break;
		}
	}

	if (!Layer)
		return;

	if (Layer->RenderCount == (int)Layer->RenderList.size())
		Layer->RenderList.resize(Layer->RenderCount * 2);

	Layer->RenderList[Layer->RenderCount] = Component;

	/*if (!Component->GetDefaultZValueSet())
	{
	}*/

	//Vector3 Pos = Component->GetWorldPos();
	//float Upper = Layer->LayerZUpperLimit;
	//float Lower = Layer->LayerZLowerLimit;

	//if (Pos.z > Upper)
	//	Pos.z = Upper;
	//if (Pos.z < Lower)
	//	Pos.z = Lower;

	//Component->SetWorldPos(Pos.x, Pos.y, Pos.z);

	++Layer->RenderCount;
}

void CRenderManager::CreateLayer(const std::string& Name, int Priority)
{
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = Name;
	Layer->LayerPriority = Priority;

	m_RenderLayerList.push_back(Layer);

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetLayerPriority(const std::string& Name, int Priority)
{
	auto	iter = m_RenderLayerList.begin();
	auto	iterEnd = m_RenderLayerList.end();

	for (; iter != iterEnd; ++iter)
	{
		if ((*iter)->Name == Name)
		{
			(*iter)->LayerPriority = Priority;
			break;
		}
	}

	sort(m_RenderLayerList.begin(), m_RenderLayerList.end(), CRenderManager::Sortlayer);
}

void CRenderManager::SetFadeAmount(float FadeAmount)
{
	m_FadeAmount = FadeAmount;
}

bool CRenderManager::Init()
{
	m_RenderStateManager = new CRenderStateManager;

	m_RenderStateManager->Init();

	m_Standard2DCBuffer = new CStandard2DConstantBuffer;

	m_Standard2DCBuffer->Init();


	// ���̾� ����
	RenderLayer* Layer = new RenderLayer;
	Layer->Name = "Stage";
	Layer->LayerPriority = 0;
	Layer->LayerZLowerLimit = 950;
	Layer->LayerZUpperLimit = 999;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Back";
	Layer->LayerPriority = 1;
	Layer->LayerZLowerLimit = 900;
	Layer->LayerZUpperLimit = 949;

	m_RenderLayerList.push_back(Layer);

	//Layer = new RenderLayer;
	//Layer->Name = "Default";
	//Layer->LayerPriority = 2;
	//Layer->LayerZOrderLowerLimit = 700;
	//Layer->LayerZOrderUpperLimit = 799;

	//m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "MapObjBackMost";
	Layer->LayerPriority = 3;
	Layer->LayerZLowerLimit = 850;
	Layer->LayerZUpperLimit = 899;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "MapObjBack";
	Layer->LayerPriority = 4;
	Layer->LayerZLowerLimit = 750;
	Layer->LayerZUpperLimit = 849;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "MapObjMiddle";
	Layer->LayerPriority = 5;
	Layer->LayerZLowerLimit = 700;
	Layer->LayerZUpperLimit = 749;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "MapObjFront";
	Layer->LayerPriority = 6;
	Layer->LayerZLowerLimit = 650;
	Layer->LayerZUpperLimit = 699;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "MovingObjFront";
	Layer->LayerPriority = 7;
	Layer->LayerZLowerLimit = 600;
	Layer->LayerZUpperLimit = 649;

	m_RenderLayerList.push_back(Layer);

	// �÷��̾ ���������� MapObj
	Layer = new RenderLayer;
	Layer->Name = "CoveringMapObj";
	Layer->LayerPriority = 8;
	Layer->LayerZLowerLimit = 500;
	Layer->LayerZUpperLimit = 599;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "Particle";
	Layer->LayerPriority = 9;
	Layer->LayerZLowerLimit = 400;
	Layer->LayerZUpperLimit = 499;

	m_RenderLayerList.push_back(Layer);

	Layer = new RenderLayer;
	Layer->Name = "ScreenWidgetComponent";
	Layer->LayerPriority = 10;
	Layer->LayerZUpperLimit = 300;
	Layer->LayerZUpperLimit = 399;

	m_RenderLayerList.push_back(Layer);



	m_DepthDisable = m_RenderStateManager->FindRenderState("DepthDisable");
	m_AlphaBlend = m_RenderStateManager->FindRenderState("AlphaBlend");


	/*CInput::GetInst()->CreateKey("FadeIn", 'O');
	CInput::GetInst()->CreateKey("FadeOut", 'P');

	CInput::GetInst()->SetKeyCallback("FadeIn", KeyState_Push, this, &CRenderManager::FadeIn);
	CInput::GetInst()->SetKeyCallback("FadeOut", KeyState_Push, this, &CRenderManager::FadeOut);*/

	return true;
}

void CRenderManager::Render()
{
	//m_DepthDisable->SetState();

	if (m_StartFadeIn)
		FadeIn(CEngine::GetInst()->GetDeltaTime());

	else if(m_StartFadeOut)
		FadeOut(CEngine::GetInst()->GetDeltaTime());

	m_Standard2DCBuffer->SetFadeAmount(m_FadeAmount);
	m_Standard2DCBuffer->UpdateCBuffer();

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			(*iter)->RenderCount = 0;
		}
	}

	{
		auto iter = m_ObjectList->begin();
		auto iterEnd = m_ObjectList->end();

		// ���� �����ȿ��� Object���� �����ִ� SceneComponent �߿���
		// m_Render = true�� SceneComponent�� m_RenderList�� �־��ش�
		for (; iter != iterEnd; ++iter)
		{
			(*iter)->PrevRender();
		}
	}

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				int Count = (*iter)->RenderCount;
				/*auto RenderListEnd = (*iter)->RenderList.begin() + Count;
				sort((*iter)->RenderList.begin(), RenderListEnd, RenderLayer::SortSceneComponent);*/

				float Tmp = m_FadeAmount;

				if (!(*iter)->RenderList[i]->GetFadeApply())
				{
					m_FadeAmount = 1.f;
					m_Standard2DCBuffer->SetFadeAmount(1.f);
					m_Standard2DCBuffer->UpdateCBuffer();
				}

				(*iter)->RenderList[i]->Render();

				if (!(*iter)->RenderList[i]->GetFadeApply())
				{
					m_FadeAmount = Tmp;

					m_Standard2DCBuffer->SetFadeAmount(m_FadeAmount);
					m_Standard2DCBuffer->UpdateCBuffer();
				}
			}
		}
	}

	{
		auto	iter = m_RenderLayerList.begin();
		auto	iterEnd = m_RenderLayerList.end();

		for (; iter != iterEnd; ++iter)
		{
			for (int i = 0; i < (*iter)->RenderCount; ++i)
			{
				//(*iter)->RenderList[i]->PostRender();

				// �߰�
				(*iter)->RenderList[i] = nullptr;

			}
			// �߰�
			//(*iter)->RenderCount = 0;
		}

	}

	m_DepthDisable->SetState();

	// Widget ���
	m_AlphaBlend->SetState();

	CSceneManager::GetInst()->GetScene()->GetViewport()->Render();

	// ���콺 ���
	CWidgetWindow* MouseWidget = CEngine::GetInst()->GetMouseWidget();

	if (MouseWidget)
		MouseWidget->Render();

	m_AlphaBlend->ResetState();

	m_DepthDisable->ResetState();
}

void CRenderManager::SetAlphaBlendState()
{
	if(m_AlphaBlend)
		m_AlphaBlend->SetState();
}

void CRenderManager::ResetAlphaBlendState()
{
	if (m_AlphaBlend)
		m_AlphaBlend->ResetState();
}

void CRenderManager::SetBlendFactor(const std::string& Name, float r, float g,
	float b, float a)
{
	m_RenderStateManager->SetBlendFactor(Name, r, g, b, a);
}

void CRenderManager::AddBlendInfo(const std::string& Name, bool BlendEnable,
	D3D11_BLEND SrcBlend, D3D11_BLEND DestBlend, D3D11_BLEND_OP BlendOp,
	D3D11_BLEND SrcBlendAlpha, D3D11_BLEND DestBlendAlpha,
	D3D11_BLEND_OP BlendOpAlpha, UINT8 RenderTargetWriteMask)
{
	m_RenderStateManager->AddBlendInfo(Name, BlendEnable, SrcBlend, DestBlend,
		BlendOp, SrcBlendAlpha, DestBlendAlpha, BlendOpAlpha, RenderTargetWriteMask);
}

bool CRenderManager::CreateBlendState(const std::string& Name,
	bool AlphaToCoverageEnable, bool IndependentBlendEnable)
{
	return m_RenderStateManager->CreateBlendState(Name, AlphaToCoverageEnable, IndependentBlendEnable);
}

CRenderState* CRenderManager::FindRenderState(const std::string& Name)
{
	return m_RenderStateManager->FindRenderState(Name);
}

bool CRenderManager::Sortlayer(RenderLayer* Src, RenderLayer* Dest)
{
	// �������� -> Priority�� ���� LayerList�� �ִ� Component��
	// �ʰ� ��µǵ��� �Ѵ�
	return Src->LayerPriority < Dest->LayerPriority;
}

void CRenderManager::FadeIn(float DeltaTime)
{
	m_FadeAmount -= DeltaTime * m_FadeInSpeed;

	if (m_FadeAmount <= m_FadeInLimit)
	{
		m_FadeAmount = m_FadeInLimit;
		m_StartFadeIn = false;

		CSceneManager::GetInst()->CallFadeInEndCallback();
	}
}

void CRenderManager::FadeOut(float DeltaTime)
{
	m_FadeAmount += DeltaTime * m_FadeOutSpeed;

	if (m_FadeAmount >= m_FadeOutLimit)
	{
		m_FadeAmount = m_FadeOutLimit;
		m_StartFadeOut = false;

		CSceneManager::GetInst()->CallFadeOutEndCallback();
	}
}
