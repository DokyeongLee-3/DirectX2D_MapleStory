#pragma once

#include "Component.h"
#include "Transform.h"

class CSceneComponent :
	public CComponent
{
	friend class CGameObject;

protected:
	CSceneComponent();
	CSceneComponent(const CSceneComponent& com);
	virtual ~CSceneComponent();

protected:
	CTransform* m_Transform;
	CSceneComponent* m_Parent;
	std::vector<CSharedPtr<CSceneComponent>> m_vecChild;
	// SceneComponent를 상속받는 Component중에
	// 화면에 출력되는 클래스(출력되는 클래스는 
	// m_Render = true)인지 아닌지를 알려주는 멤버
	// 출력된다면 Mesh와 Material(Material은 Shader를 갖고 있음)
	// 를 갖고 있어야 한다
	bool    m_Render;
	// 어떤 Layer에 속해 있는지
	std::string		m_LayerName;
	int m_ZOrder;
	// FadeIn/Out 효과시 적용을 받을건지 말건지
	bool	m_FadeApply;
	bool	m_DefaultZValueSet;

public:
	void DefaultZValueSet(bool Set)
	{
		m_DefaultZValueSet = Set;
	}

	bool GetDefaultZValueSet()	const
	{
		return m_DefaultZValueSet;
	}

	bool GetFadeApply()	const
	{
		return m_FadeApply;
	}

	void SetFadeApply(bool Apply)
	{
		m_FadeApply = Apply;
	}

	CSceneComponent* GetParent()	const
	{
		return m_Parent;
	}

	int GetZOrder()	const
	{
		return m_ZOrder;
	}

	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	std::string GetLayerName()	const
	{
		return m_LayerName;
	}

	void SetRender(bool Render)
	{
		m_Render = Render;
	}

	bool IsRender() const
	{
		return m_Render;
	}

	int GetChildCount()	const
	{
		return (int)m_vecChild.size();
	}

public:
	void SetLayerName(const std::string& Name);

	void SetAllChildComponentScene(class CScene* Scene);

public:
	// Obj에 m_SceneComponentList에 나와 내 자식 컴포넌트들을 넣어준다
	void SetSceneComponent(class CGameObject* Object);
	void GetAllSceneComponentsName(std::vector<FindComponentName>& vecNames);
	virtual void Destroy();

public:
	virtual void SetScene(class CScene* Scene);
	virtual void SetGameObject(class CGameObject* Object);

public:
	void AddChild(CSceneComponent* Child);
	bool DeleteChild(CSceneComponent* Child);
	bool DeleteChild(const std::string& Name);
	void ClearChild();
	CSceneComponent* FindComponent(const std::string& Name);
	void SetAllSceneComponentsLayer(const std::string& Name);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void CheckCollision();
	virtual void PrevRender();
	// Transform정보를 담고 있는 상수버퍼를 
	// 렌더 직전에 셰이더로 넘겨준다
	virtual void Render();
	virtual void PostRender();
	virtual CSceneComponent* Clone();
	virtual void Save(FILE* File);
	virtual void Load(FILE* File);


public:
	void SetInheritScale(bool Inherit)
	{
		m_Transform->SetInheritScale(Inherit);
	}

	void SetInheritRotX(bool Inherit)
	{
		m_Transform->SetInheritRotX(Inherit);
	}

	void SetInheritRotY(bool Inherit)
	{
		m_Transform->SetInheritRotY(Inherit);
	}

	void SetInheritRotZ(bool Inherit)
	{
		m_Transform->SetInheritRotZ(Inherit);
	}

	void SetInheritParentRotationPosX(bool Inherit)
	{
		m_Transform->SetInheritParentRotationPosX(Inherit);
	}

	void SetInheritParentRotationPosY(bool Inherit)
	{
		m_Transform->SetInheritParentRotationPosY(Inherit);
	}

	void SetInheritParentRotationPosZ(bool Inherit)
	{
		m_Transform->SetInheritParentRotationPosZ(Inherit);
	}

	void InheritScale(bool Current)
	{
		m_Transform->InheritScale(Current);
	}

	void InheritRotation(bool Current)
	{
		m_Transform->InheritRotation(Current);
	}

	void InheritParentRotationPos(bool Current)
	{
		m_Transform->InheritParentRotationPos(Current);
	}

	void InheritWorldScale(bool Current)
	{
		m_Transform->InheritWorldScale(Current);
	}

	void InheritWorldRotation(bool Current)
	{
		m_Transform->InheritWorldRotation(Current);
	}

public:
	Vector3 GetRelativeScale()	const
	{
		return m_Transform->GetRelativeScale();
	}

	Vector3 GetRelativeRot()	const
	{
		return m_Transform->GetRelativeRot();
	}

	Vector3 GetRelativePos()	const
	{
		return m_Transform->GetRelativePos();
	}

	Vector3 GetRelativeAxis(AXIS Axis)
	{
		return m_Transform->GetRelativeAxis(Axis);
	}

public:
	void SetRelativeScale(const Vector3& Scale)
	{
		m_Transform->SetRelativeScale(Scale);
	}

	void SetRelativeScale(float x, float y, float z)
	{
		m_Transform->SetRelativeScale(x, y, z);
	}

	void SetRelativeRotation(const Vector3& Rot)
	{
		m_Transform->SetRelativeRotation(Rot);
	}

	void SetRelativeRotation(float x, float y, float z)
	{
		m_Transform->SetRelativeRotation(x, y, z);
	}

	void SetRelativeRotationX(float x)
	{
		m_Transform->SetRelativeRotationX(x);
	}

	void SetRelativeRotationY(float y)
	{
		m_Transform->SetRelativeRotationY(y);
	}

	void SetRelativeRotationZ(float z)
	{
		m_Transform->SetRelativeRotationZ(z);
	}

	void SetRelativePos(const Vector3& Pos)
	{
		m_Transform->SetRelativePos(Pos);
	}

	void SetRelativePos(float x, float y, float z)
	{
		m_Transform->SetRelativePos(x, y, z);
	}

	void AddRelativeScale(const Vector3& Scale)
	{
		m_Transform->AddRelativeScale(Scale);
	}

	void AddRelativeScale(float x, float y, float z)
	{
		m_Transform->AddRelativeScale(x, y, z);
	}

	void AddRelativeRotation(const Vector3& Rot)
	{
		m_Transform->AddRelativeRotation(Rot);
	}

	void AddRelativeRotation(float x, float y, float z)
	{
		m_Transform->AddRelativeRotation(x, y, z);
	}

	void AddRelativeRotationX(float x)
	{
		m_Transform->AddRelativeRotationX(x);
	}

	void AddRelativeRotationY(float y)
	{
		m_Transform->AddRelativeRotationY(y);
	}

	void AddRelativeRotationZ(float z)
	{
		m_Transform->AddRelativeRotationZ(z);
	}

	void AddRelativePos(const Vector3& Pos)
	{
		m_Transform->AddRelativePos(Pos);
	}

	void AddRelativePos(float x, float y, float z)
	{
		m_Transform->AddRelativePos(x, y, z);
	}

public:
	Vector3 GetWorldScale()	const
	{
		return m_Transform->GetWorldScale();
	}

	Vector3 GetWorldRot()	const
	{
		return m_Transform->GetWorldRot();
	}

	Vector3 GetWorldPos()	const
	{
		return m_Transform->GetWorldPos();
	}

	Vector3 GetPivot()	const
	{
		return m_Transform->GetPivot();
	}

	Vector3 GetMeshSize()	const
	{
		return m_Transform->GetMeshSize();
	}

	const Matrix& GetWorldMatrix()	const
	{
		return m_Transform->GetWorldMatrix();
	}

	Vector3 GetWorldAxis(AXIS Axis)
	{
		return m_Transform->GetWorldAxis(Axis);
	}

public:
	void SetPivot(const Vector3& Pivot)
	{
		m_Transform->SetPivot(Pivot);
	}

	void SetPivot(float x, float y, float z)
	{
		m_Transform->SetPivot(x, y, z);
	}

	void SetMeshSize(const Vector3& Size)
	{
		m_Transform->SetMeshSize(Size);
	}

	void SetMeshSize(float x, float y, float z)
	{
		m_Transform->SetMeshSize(x, y, z);
	}

public:
	void SetWorldScale(const Vector3& Scale)
	{
		m_Transform->SetWorldScale(Scale);
	}

	void SetWorldScale(float x, float y, float z)
	{
		m_Transform->SetWorldScale(x, y, z);
	}

	void SetWorldRotation(const Vector3& Rot)
	{
		m_Transform->SetWorldRotation(Rot);
	}

	void SetWorldRotation(float x, float y, float z)
	{
		m_Transform->SetWorldRotation(x, y, z);
	}

	void SetWorldRotationX(float x)
	{
		m_Transform->SetWorldRotationX(x);
	}

	void SetWorldRotationY(float y)
	{
		m_Transform->SetWorldRotationY(y);
	}

	void SetWorldRotationZ(float z)
	{
		m_Transform->SetWorldRotationZ(z);
	}

	void SetWorldPos(const Vector3& Pos)
	{
		m_Transform->SetWorldPos(Pos);
	}

	void SetWorldPos(float x, float y, float z)
	{
		m_Transform->SetWorldPos(x, y, z);
	}

	void AddWorldScale(const Vector3& Scale)
	{
		m_Transform->AddWorldScale(Scale);
	}

	void AddWorldScale(float x, float y, float z)
	{
		m_Transform->AddWorldScale(x, y, z);
	}

	void AddWorldRotation(const Vector3& Rot)
	{
		m_Transform->AddWorldRotation(Rot);
	}

	void AddWorldRotation(float x, float y, float z)
	{
		m_Transform->AddWorldRotation(x, y, z);
	}

	void AddWorldRotationX(float x)
	{
		m_Transform->AddWorldRotationX(x);
	}

	void AddWorldRotationY(float y)
	{
		m_Transform->AddWorldRotationY(y);
	}

	void AddWorldRotationZ(float z)
	{
		m_Transform->AddWorldRotationZ(z);
	}

	void AddWorldPos(const Vector3& Pos)
	{
		m_Transform->AddWorldPos(Pos);
	}

	void AddWorldPos(float x, float y, float z)
	{
		m_Transform->AddWorldPos(x, y, z);
	}
};

