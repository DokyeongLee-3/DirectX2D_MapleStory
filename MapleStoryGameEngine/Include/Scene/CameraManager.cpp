
#include "CameraManager.h"

CCameraManager::CCameraManager()
{
}

CCameraManager::~CCameraManager()
{
}

void CCameraManager::Start()
{
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->Start();

	m_UICamera->Start();
}

void CCameraManager::Init()
{
	m_CurrentCamera = new CCameraComponent;

	m_CurrentCamera->Init();

	m_CurrentCamera->SetName("DefaultCamera");
	m_CurrentCamera->m_Scene = m_Scene;
	m_CurrentCamera->SetCameraType(Camera_Type::Camera2D);

	m_UICamera = new CCameraComponent;

	m_UICamera->Init();

	m_UICamera->SetName("UICamera");
	m_UICamera->m_Scene = m_Scene;
	m_UICamera->SetCameraType(Camera_Type::CameraUI);

}

void CCameraManager::Update(float DeltaTime)
{
	// 여기선 Default카메라만 업데이트해주고, 새로 만들어서 사용할 카메라는 CurrentCamera로
	// 설정하고, 그 CurrentCamera는 CTransform::SetTransform에서 매 프레임 뷰행렬을 업데이트 해준다
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->Update(DeltaTime);
}

void CCameraManager::PostUpdate(float DeltaTime)
{
	if (m_CurrentCamera->GetName() == "DefaultCamera")
		m_CurrentCamera->PostUpdate(DeltaTime);
}
