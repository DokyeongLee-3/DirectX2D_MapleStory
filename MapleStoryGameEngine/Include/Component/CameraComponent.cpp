
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
	카메라가 뷰공간으로 변환이 완료되면 카메라를 구성하는 x, y, z 축은 월드의 축과 일치하게
	된다.즉, x축은 (1, 0, 0) y축은 (0, 1, 0) z축은 (0, 0, 1)이 된다.
	*/
	CSceneComponent::Update(DeltaTime);
}

void CCameraComponent::PostUpdate(float DeltaTime)
{
	// 플레이어같은 오브젝트들의 이동이나 회전 정보가
	// 갱신되고나서 카메라의 위치가 갱신되어야해서(ex. 플레이어에 카메라가 고정된 경우라면
	// 플레이어의 Component위치에 따라서 CameraComponent도 똑같이 따라가야하니까)
	// PostUpdate에서 해준다
	CSceneComponent::PostUpdate(DeltaTime);

	m_matView.Identity();


	// 뷰 행렬은 카메라의 회전행렬의 역행렬(R^-1)과 
	// 이동 행렬의 역행렬(T^-1)의 곱으로 이루어져있음
	for (int i = 0; i < AXIS_MAX; ++i)
	{
		// Axis에는 카메라의 최종적인 회전 행렬이 있음
		Vector3	Axis = GetWorldAxis((AXIS)i);
		memcpy(&m_matView[i][0], &Axis, sizeof(Vector3));
	}


	m_matView.Transpose();

	Vector3	Pos = GetWorldPos() * -1.f;

	// 카메라 회전행렬의 역행렬과 이동행렬의 역행렬을 곱해보면
	// 4x1 성분은 (위의 for문에서 얻은)X축과 카메라의 이동벡터의
	// 내적에 -1을 곱한것, 4x2 성분은 Y축과 카메라 이동벡터의 내적에
	// -1을 곱한것, 4x3은 Z축과 카메라 이동벡터에 -1을 곱한것과
	// 같다는 것을 확인할 수 있다. 따라서 아래와 같이 뷰행렬을 만들어준다
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
