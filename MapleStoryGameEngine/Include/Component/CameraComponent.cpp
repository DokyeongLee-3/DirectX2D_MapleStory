
#include "CameraComponent.h"
#include "../Device.h"

CCameraComponent::CCameraComponent()
{
	SetTypeID<CCameraComponent>();
	m_Render = false;

	m_CameraType = Camera_Type::Camera2D;
	m_ViewAngle = 90.f;
	m_Distance = 1000.f;
}

CCameraComponent::CCameraComponent(const CCameraComponent& com) :
	CSceneComponent(com)
{
	m_CameraType = com.m_CameraType;
	m_matView = com.m_matView;
	m_matProj = com.m_matProj;
	m_ViewAngle = com.m_ViewAngle;
	m_Distance = com.m_Distance;
	m_RS = com.m_RS;
}

CCameraComponent::~CCameraComponent()
{
}

void CCameraComponent::CreateProjectionMatrix()
{
	switch (m_CameraType)
	{
	case Camera_Type::Camera2D:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	case Camera_Type::Camera3D:
		m_matProj = XMMatrixPerspectiveFovLH(DegreeToRadian(m_ViewAngle),
			m_RS.Width / (float)m_RS.Height, 0.1f, m_Distance);
		break;
	case Camera_Type::CameraUI:
		m_matProj = XMMatrixOrthographicOffCenterLH(0.f, (float)m_RS.Width, 0.f, (float)m_RS.Height, 0.f, 1000.f);
		break;
	}

}

void CCameraComponent::Start()
{
	CSceneComponent::Start();

	CreateProjectionMatrix();
}

bool CCameraComponent::Init()
{
	m_RS = CDevice::GetInst()->GetResolution();

	return true;
}

void CCameraComponent::Update(float DeltaTime)
{
	/*
	ī�޶� ��������� ��ȯ�� �Ϸ�Ǹ� ī�޶� �����ϴ� x, y, z ���� ������ ��� ��ġ�ϰ�
	�ȴ�.��, x���� (1, 0, 0) y���� (0, 1, 0) z���� (0, 0, 1)�� �ȴ�.
	*/
	CSceneComponent::Update(DeltaTime);
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	// �÷��̾�� ������Ʈ���� �̵��̳� ȸ�� ������
	// ���ŵǰ��� ī�޶��� ��ġ�� ���ŵǾ���ؼ�(ex. �÷��̾ ī�޶� ������ �����
	// �÷��̾��� Component��ġ�� ���� CameraComponent�� �Ȱ��� ���󰡾��ϴϱ�)
	// PostUpdate���� ���ش�
	CSceneComponent::PostUpdate(DeltaTime);

	m_matView.Identity();


	// �� ����� ī�޶��� ȸ������� �����(R^-1)�� 
	// �̵� ����� �����(T^-1)�� ������ �̷��������
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		// Axis���� ī�޶��� �������� ȸ�� ����� ����
		Vector3	Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}


	m_matView.Transpose();

	Vector3	Pos = GetWorldPos() * -1.f;

	// ī�޶� ȸ������� ����İ� �̵������ ������� ���غ���
	// 4x1 ������ (���� for������ ����)X��� ī�޶��� �̵�������
	// ������ -1�� ���Ѱ�, 4x2 ������ Y��� ī�޶� �̵������� ������
	// -1�� ���Ѱ�, 4x3�� Z��� ī�޶� �̵����Ϳ� -1�� ���ѰͰ�
	// ���ٴ� ���� Ȯ���� �� �ִ�. ���� �Ʒ��� ���� ������� ������ش�
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		Vector3	Axis = GetWorldAxis((AXIS)i);

		m_matView[3][i] = Pos.Dot(Axis);
	}
}

void CCameraComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CCameraComponent::Render()
{
	CSceneComponent::Render();
}

void CCameraComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CCameraComponent* CCameraComponent::Clone()
{
	return new CCameraComponent(*this);
}

void CCameraComponent::Save(FILE* File)
{
	fwrite(&m_CameraType, sizeof(Camera_Type), 1, File);
	fwrite(&m_matView, sizeof(Matrix), 1, File);
	fwrite(&m_matProj, sizeof(Matrix), 1, File);
	fwrite(&m_ViewAngle, sizeof(float), 1, File);
	fwrite(&m_Distance, sizeof(float), 1, File);
	fwrite(&m_RS, sizeof(Resolution), 1, File);

	CSceneComponent::Save(File);
}

void CCameraComponent::Load(FILE* File)
{
	fread(&m_CameraType, sizeof(Camera_Type), 1, File);
	fread(&m_matView, sizeof(Matrix), 1, File);
	fread(&m_matProj, sizeof(Matrix), 1, File);
	fread(&m_ViewAngle, sizeof(float), 1, File);
	fread(&m_Distance, sizeof(float), 1, File);
	fread(&m_RS, sizeof(Resolution), 1, File);

	CSceneComponent::Load(File);
}
