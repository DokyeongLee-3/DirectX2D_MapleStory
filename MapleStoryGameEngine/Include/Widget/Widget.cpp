
#include "Widget.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Scene/CameraManager.h"
#include "../Component/CameraComponent.h"
#include "../Resource/Shader/WidgetConstantBuffer.h"

CWidget::CWidget() :
	m_Owner(nullptr),
	m_ZOrder(0),
	m_Start(false),
	m_Angle(0.f),
	m_CBuffer(nullptr),
	m_Size(50.f, 50.f),
	m_MouseHovered(false)
{
}

CWidget::CWidget(const CWidget& widget)
{
	m_MouseHovered = false;
}

CWidget::~CWidget()
{
	SAFE_DELETE(m_CBuffer);
}

void CWidget::SetShader(const std::string& Name)
{
	m_Shader = m_Owner->GetViewport()->GetScene()->GetResource()->FindShader(Name);
}

void CWidget::SetUseTexture(bool Use)
{
	m_CBuffer->SetUseTexture(Use);
}

void CWidget::Start()
{
	m_Start = true;
}

bool CWidget::Init()
{
	m_Shader = m_Owner->GetViewport()->GetScene()->GetResource()->FindShader("WidgetShader");
	m_Mesh = m_Owner->GetViewport()->GetScene()->GetResource()->FindMesh("WidgetMesh");

	m_CBuffer = new CWidgetConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CWidget::Update(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::PostUpdate(float DeltaTime)
{
	if (!m_Start)
		Start();
}

void CWidget::Render()
{
	if (!m_Start)
		Start();

	Matrix	matScale, matRot, matTrans;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matRot.Rotation(0.f, 0.f, m_Angle);
	matTrans.Translation(m_Pos.x, m_Pos.y, 0.f);

	CCameraComponent* UICamera = m_Owner->GetViewport()->GetScene()->GetCameraManager()->GetUICamera();

	Matrix	matWP = matScale * matRot * matTrans * UICamera->GetProjMatrix();
	matWP.Transpose();

	m_CBuffer->SetWP(matWP);
	m_CBuffer->SetTint(m_Tint);

	m_CBuffer->UpdateCBuffer();

	m_Shader->SetShader();

	m_Mesh->Render();
}

bool CWidget::CollisionMouse(const Vector2& MousePos)
{
	if (m_Pos.x > MousePos.x)
		return false;

	else if (m_Pos.x + m_Size.x < MousePos.x)
		return false;

	else if (m_Pos.y > MousePos.y)
		return false;

	else if (m_Pos.y + m_Size.y < MousePos.y)
		return false;

	return true;
}
