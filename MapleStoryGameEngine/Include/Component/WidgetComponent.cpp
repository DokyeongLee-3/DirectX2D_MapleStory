
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

// WidgetComponent�� Player���� ������Ʈ�� �ٿ��� ���°�(Viewport���� ����°� �ƴ϶�)�� 
// �����ִ� m_WidgetWindow�� Update(Ȥ�� PostUpdate)�� ���� �� ����� �Ѵ�
void CWidgetComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);

	if (m_WidgetWindow)
		m_WidgetWindow->Update(DeltaTime);
}

// WidgetComponent�� Player���� ������Ʈ�� �ٿ��� ���°�(Viewport���� ����°� �ƴ϶�)�� 
// �����ִ� m_WidgetWindow�� Update(Ȥ�� PostUpdate)�� ���� �� ����� �Ѵ�
// ���⼭ WidgetComponent�� RenderPos�� ���� �������� �ϴ� ������ WidgetComponent�� Ư�������̴�
// �Ϲ� �κ��丮â���� Widget�� ������ Viewport�� �־ ���� UIó�� �׳� �ػ� �� ȭ����ǥ�� ��Ƽ�
// View��ȯ���� ��µǰ��ϸ� ī�޶� �÷��̾ ���� �����̴� ���� ��� ȭ�鳻���� ������ ��ġ�� ���
// ��µǰ� �ɰ�������, WidgetComponent�� �ٸ���. ���� ��� ���� �ؿ� �̸��� HPBar Widget�� �ִٸ�
// ī�޶� �����ӿ�����(Ȥ�� ���Ͱ� �����ӿ� ����) �� ������ Widget���� ȭ�鳻���� ���̱⵵, ȭ�� ������ ������
// �Ⱥ��̱⵵ �� ���̴�. ���� �⺻������ World��ǥ�� ������, �� World��ǥ�� Screen��ǥ�� ��ȯ�ؼ�
// Render�� �������� Widget::Render�� �״�� �� �� �ֵ��� �Ѵ�. World��ǥ�� Screen��ǥ�� ��ȯ�ϴ� ��������
// World, View, Projection�� ��� �����ϰ� Screen��ǥ�� ��ȯ�ϴ� ����(�Ʒ����� �ػ� ���ݸ�ŭ ���ϰ� ���� �����ִ� ����)��
// ��ġ�� ī�޶� �ۿ� �ִ� WidgetComponent���� x��ǥ�� 0���� �۰ų� 1280���� ũ�ų� y��ǥ�� 0���� �۰ų� 720���� ũ�ų�
// �̷��� �ż� ������ ��½� ������ �ʰ� �ɰ��̴�
void CWidgetComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);

	/*if (CRenderManager::GetInst()->GetStartFadeIn())
		return;*/

	if (m_WidgetWindow)
	{
		// ������������� ��ġ ������ WidgetComponent�� ��ġ ������ ���ŵǾ� ���� ���̴�.
		// �̸� ȭ����������� ��ġ�� ��ȯ�� �־�� �Ѵ�.
		// ���� -> �� -> ���� ��ȯ �Ŀ� ȭ��������� ��ȯ�� �ȴ�.
		// ȭ��������� ��ȯ�� �ɶ����� �������� ��ȯ�� x, y, z, w �� ���� ��� ��� ��ҵ���
		// w�� ����� ȭ��������� ��ȯ�� �õ��Ѵ�.
		CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();

		Matrix	matView = Camera->GetViewMatrix();
		Matrix	matProj = Camera->GetProjMatrix();
		Matrix	matVP = matView * matProj;

		Vector3	WorldPos = GetWorldPos();

		Vector3	ViewPos = WorldPos.TransformCoord(matView);

		// 2D���� XMMatrixOrthographicOffCenterLH�� �������� ����ε� Ȯ���غ��� 11 22 33 ������ 1�� �ƴ϶� 0.001�̳� 0.002���� ���� ���� ������ �Ǿ��ִµ�
		// �̴� ���ϸ� x, y���� �ٷ� NDC�� ���εǰ� �ϱ� �����̴�
		// XMMatrixOrthographicOffCenterLH�� �����ָ� �ٷ� NDC�� ���εȴ�(x, y��[-1, 1]�� ����) ���ٳ������ ������������ ������ z�� 1�̶� �ǹ̾���
		// ���� XMMatrixOrthographicOffCenterLH�� ���ϸ� ������ NDC�� ������ ��
		// ����� XMMatrixOrthographicOffCenterLH�Լ��� ȭ�鿡�� ���߾�, �׷��ϱ� ���� ��������
		// (640, 360)��ġ�� ī�޶� �����̶�� �����ϰ� �װ� �������� x,y�� ������
		// ������ǥ���� [-1, 1]�� ���ν�Ų��. ���� ��� ViewPos�� (590.f, 410.f)�̸� XMMatrixOrthographicOffCenterLH�� ������� ����
		// ������Ű�� ((590.f - 640.f) / 640.f, (410.f - 360.f) / 360.f) = (-0.078125f, 0.138888f)�� �ȴ�
		Vector3	ProjPos = ViewPos.TransformCoord(matProj);

		// Q) �� WorldSpace�� Widget Component�� ���ٳ����⸦ ���ִ°�?
		// A) 3D���� ����� ����ν�, ������ ����->��->����->���ٳ����� ���� ���ִ°� �´µ�
		// ScreenŸ���� World�� ��ġ�Ǵ°� �ƴ϶� ����� �����Ǵ� ��ũ���̶�� ������ 2D��
		// ��µǴ� �����ν�, ���ٳ����Ⱑ �ʿ����
		if (m_Space == WidgetComponent_Space::World)
		{
			float	w = ViewPos.x * matProj._14 + ViewPos.y * matProj._24 + ViewPos.z * matProj._34 + matProj._44;

			ProjPos.x /= w;
			ProjPos.y /= w;
			// 2D�϶��� �������� ����� 14 24 34 element�� 0�̰� 44�� 1�̶� ���� ���� w�� ������ 1
			ProjPos.z /= w;
		}

		Resolution	RS = CDevice::GetInst()->GetResolution();

		Vector2	ScreenPos;

		ScreenPos.x = ProjPos.x * (RS.Width / 2.f) + (RS.Width / 2.f);
		ScreenPos.y = ProjPos.y * (RS.Height / 2.f) + (RS.Height / 2.f);

		/*if (CEngine::GetInst()->GetEngineSpace() == Engine_Space::Space2D)
		{

		}

		// ȭ�� ���������� �Ʒ� ������ +y�̴�
		// ����->��->����->ȭ����ǥ ��ȯ�� https://secretroute.tistory.com/entry/1407210828 ����
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
