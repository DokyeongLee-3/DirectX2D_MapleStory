#pragma once
#include "SceneComponent.h"
class CCameraComponent :
    public CSceneComponent
{
	friend class CGameObject;
	friend class CCameraManager;

protected:
	CCameraComponent();
	CCameraComponent(const CCameraComponent& com);
	virtual ~CCameraComponent();

protected:
	Camera_Type m_CameraType;
	Matrix      m_matView;
	Matrix      m_matProj;
	float		m_ViewAngle;
	float		m_Distance;
	Resolution	m_RS;
	Vector2		m_Ratio;

public:
	Resolution GetResolution()	const
	{
		return m_RS;
	}

	Camera_Type GetCameraType()	const
	{
		return m_CameraType;
	}

	Matrix GetViewMatrix()	const
	{
		return m_matView;
	}

	Matrix GetProjMatrix()	const
	{
		return m_matProj;
	}

	Vector2 GetLeftBottom()	const
	{
		Vector2	LB;

		LB.x = GetWorldPos().x;// - m_Ratio.x * m_RS.Width;
		LB.y = GetWorldPos().y;// - m_Ratio.y * m_RS.Height;

		return LB;
	}

	Vector2	GetRatio()	const
	{
		return m_Ratio;
	}

public:
	void SetViewAngle(float Angle)
	{
		m_ViewAngle = Angle;

		if (m_CameraType == Camera_Type::Camera3D)
			CreateProjectionMatrix();
	}

	void SetDistance(float Distance)
	{
		m_Distance = Distance;

		CreateProjectionMatrix();
	}

	void SetCameraType(Camera_Type Type)
	{
		m_CameraType = Type;

		CreateProjectionMatrix();
	}

	// 2D 전용
	void OnViewportCenter()
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			// 반드시 RelativePos로 써야하는데 이유는
			// 만약 이 CameraComponent를 갖고 있는 오브젝트의
			// WorldPos가 (100.f, 50.f)인 상태에서 이 오브젝트가
			// 중심에 오도록 카메라를 세팅하고 싶으면 해상도(1280.f, 720.f)의
			// 절반인 (640.f, 360.f)에서 (100.f, 50.f)를 뺀 (540.f, 310.f)만큼만
			// 카메라 뷰 행렬을 구성해야하기 때문이다.
			// 이건 아래 SetViewportRatio함수에도 마찬가지로 적용되는 개념이다
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width / -2.f, m_RS.Height / -2.f, z);

			m_Ratio.x = 0.5f;
			m_Ratio.y = 0.5f;
		}
	}

	// 2D 전용
	void SetViewportRatio(float x, float y)
	{
		if (m_CameraType == Camera_Type::Camera2D)
		{
			float	z = GetRelativePos().z;
			SetRelativePos(m_RS.Width * -x, m_RS.Height * -y, z);

			m_Ratio.x = x;
			m_Ratio.y = y;
		}
	}

private:
	void CreateProjectionMatrix();

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void PrevRender();
	virtual void Render();
	virtual void PostRender();
	virtual CCameraComponent* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);
};

