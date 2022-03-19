
#include "WidgetComponent.h"
#include "CameraComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/CameraManager.h"
#include "../Device.h"
#include "../Render/RenderManager.h"
#include "../Input.h"

CWidgetComponent::CWidgetComponent() :
	m_SetAlphaBlendState(false)
{
	SetTypeID<CWidgetComponent>();

	m_Space = WidgetComponent_Space::Screen;

	SetInheritRotX(false);
	SetInheritRotY(false);
	SetInheritRotZ(false);

	m_Render = true;

	m_FadeApply = false;
}

CWidgetComponent::CWidgetComponent(const CWidgetComponent& com) :
	CSceneComponent(com)
{
	m_Space = com.m_Space;

	if (m_Space == WidgetComponent_Space::Screen)
	{
		SetInheritRotX(false);
		SetInheritRotY(false);
		SetInheritRotZ(false);
	}

	else
	{
		SetInheritRotX(true);
		SetInheritRotY(true);
		SetInheritRotZ(true);
	}

	m_WidgetWindow = com.m_WidgetWindow->Clone();
	m_WidgetWindow->m_OwnerComponent = this;
}

CWidgetComponent::~CWidgetComponent()
{
}

void CWidgetComponent::Start()
{
	CSceneComponent::Start();
}

bool CWidgetComponent::Init()
{
	SetLayerName("ScreenWidgetComponent");

	return true;
}

// WidgetComponent는 Player같은 오브젝트에 붙여서 쓰는거(Viewport에서 만드는게 아니라)라서 
// 갖고있는 m_WidgetWindow의 Update(혹은 PostUpdate)를 따로 다 해줘야 한다
void CWidgetComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_WidgetWindow)
		m_WidgetWindow->Update(DeltaTime);
}

// WidgetComponent는 Player같은 오브젝트에 붙여서 쓰는거(Viewport에서 만드는게 아니라)라서 
// 갖고있는 m_WidgetWindow의 Update(혹은 PostUpdate)를 따로 다 해줘야 한다
// 여기서 WidgetComponent의 RenderPos를 따로 만들어줘야 하는 이유는 WidgetComponent의 특성때문이다
// 일반 인벤토리창같은 Widget은 기존에 Viewport에 넣어서 만든 UI처럼 그냥 해상도 내 화면좌표로 잡아서
// View변환없이 출력되게하면 카메라가 플레이어를 따라 움직이던 말던 계속 화면내에서 일정한 위치에 계속
// 출력되게 될것이지만, WidgetComponent는 다르다. 예를 들어 몬스터 밑에 이름과 HPBar Widget이 있다면
// 카메라가 움직임에따라(혹은 몬스터가 움직임에 따라) 이 몬스터의 Widget들은 화면내에서 보이기도, 화면 밖으로 나가서
// 안보이기도 할 것이다. 따라서 기본적으로 World좌표를 가지되, 이 World좌표를 Screen좌표로 변환해서
// Render는 만들어놓은 Widget::Render를 그대로 쓸 수 있도록 한다. World좌표를 Screen좌표로 변환하는 과정에서
// World, View, Projection을 모두 적용하고 Screen좌표로 변환하는 과정(아래에서 해상도 절반만큼 곱하고 절반 더해주는 과정)을
// 거치면 카메라 밖에 있는 WidgetComponent들은 x좌표가 0보다 작거나 1280보다 크거나 y좌표가 0보다 작거나 720보다 크거나
// 이렇게 돼서 저절로 출력시 보이지 않게 될것이다
void CWidgetComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	/*if (CRenderManager::GetInst()->GetStartFadeIn())
		return;*/

	if (m_WidgetWindow)
	{
		// 월드공간에서의 위치 정보로 WidgetComponent의 위치 정보가 갱신되어 있을 것이다.
		// 이를 화면공간에서의 위치로 변환해 주어야 한다.
		// 월드 -> 뷰 -> 투영 변환 후에 화면공간으로 변환이 된다.
		// 화면공간으로 변환이 될때에는 투영으로 변환된 x, y, z, w 가 있을 경우 모든 요소들을
		// w로 나누어서 화면공간으로 변환을 시도한다.
		CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix	matView = Camera->GetViewMatrix();
		Matrix	matProj = Camera->GetProjMatrix();
		Matrix	matVP = matView * matProj;

		Vector3	WorldPos = GetWorldPos();

		Vector3	ViewPos = WorldPos.TransformCoord(matView);

		// 2D에서 XMMatrixOrthographicOffCenterLH은 직교투영 행렬인데 확인해보면 11 22 33 성분이 1이 아니라 0.001이나 0.002같은 아주 작은 값으로 되어있는데
		// 이는 곱하면 x, y값이 바로 NDC로 매핑되게 하기 위함이다
		// XMMatrixOrthographicOffCenterLH를 곱해주면 바로 NDC로 매핑된다(x, y가[-1, 1]에 존재) 원근나누기는 직교투영에서 어차피 z가 1이라서 의미없다
		// 따라서 XMMatrixOrthographicOffCenterLH만 곱하면 투영과 NDC로 매핑이 끝
		// 참고로 XMMatrixOrthographicOffCenterLH함수는 화면에서 정중앙, 그러니까 지금 설정에선
		// (640, 360)위치가 카메라 원점이라고 생각하고 그걸 기준으로 x,y의 범위를
		// 정규좌표계인 [-1, 1]로 매핑시킨다. 예를 들어 ViewPos가 (590.f, 410.f)이면 XMMatrixOrthographicOffCenterLH로 투영행렬 만들어서
		// 투영시키면 ((590.f - 640.f) / 640.f, (410.f - 360.f) / 360.f) = (-0.078125f, 0.138888f)가 된다
		Vector3	ProjPos = ViewPos.TransformCoord(matProj);

		// Q) 왜 WorldSpace의 Widget Component만 원근나누기를 해주는가?
		// A) 3D까지 고려한 설계로써, 원래는 월드->뷰->투영->원근나누기 까지 해주는게 맞는데
		// Screen타입은 World상에 배치되는게 아니라 월드와 구별되는 스크린이라는 한장의 2D에
		// 출력되는 것으로써, 원근나누기가 필요없다
		if (m_Space == WidgetComponent_Space::World)
		{
			float	w = ViewPos.x * matProj._14 + ViewPos.y * matProj._24 + ViewPos.z * matProj._34 + matProj._44;

			ProjPos.x /= w;
			ProjPos.y /= w;
			// 2D일때는 직교투영 행렬의 14 24 34 element가 0이고 44가 1이라서 위의 변수 w는 무조건 1
			ProjPos.z /= w;
		}

		Resolution	RS = CDevice::GetInst()->GetResolution();

		Vector2	ScreenPos;

		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
		ScreenPos.y = ProjPos.y * (RS.Height / 2.f) + (RS.Height / 2.f);

		/*if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		{

		}

		// 화면 공간에서는 아래 방향이 +y이다
		// 월드->뷰->투영->화면좌표 변환은 https://secretroute.tistory.com/entry/1407210828 참고
		else
		{
			ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
			ScreenPos.y = ProjPos.y * (RS.Height / -2.f) + (RS.Height / 2.f);
		}*/


		m_WidgetWindow->SetPos(ScreenPos);

		m_WidgetWindow->PostUpdate(DeltaTime);
	}
}

void CWidgetComponent::PrevRender()
{
	/*if (CRenderManager::GetInst()->GetStartFadeIn())
		return;*/

	if(m_WidgetWindow->IsMouseCollisionEnable())
	{
		Vector2 MousePos = CInput::GetInst()->GetMousePos();

		if (m_WidgetWindow->CollisionMouse(MousePos))
		{
			int a = 3;
		}
	}

	CSceneComponent::PrevRender();
}

void CWidgetComponent::Render()
{
	/*if (CRenderManager::GetInst()->GetStartFadeIn())
		return;*/

	CSceneComponent::Render();

	if(m_SetAlphaBlendState)
		CRenderManager::GetInst()->SetAlphaBlendState();

	m_WidgetWindow->Render();

	if (m_SetAlphaBlendState)
		CRenderManager::GetInst()->ResetAlphaBlendState();
}

void CWidgetComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CWidgetComponent* CWidgetComponent::Clone()
{
	return new CWidgetComponent(*this);
}

void CWidgetComponent::Save(FILE* File)
{

	CSceneComponent::Save(File);
}

void CWidgetComponent::Load(FILE* File)
{

	CSceneComponent::Load(File);
}
