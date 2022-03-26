#pragma once

#include "../GameInfo.h"

struct CollisionSectionInfo
{
	std::vector<class CCollisionSection*>	vecSection;
	Vector3	SectionSize;		// ���� 1ĭ�� ũ��
	Vector3	Center;				// ��ü ���������� �߽�
	Vector3	SectionTotalSize;	// ��ü ���� ũ��
	Vector3	Min;				// ��ü ���������� ���ϴ� ��ǥ��
	Vector3	Max;				// ��ü ���������� ���� ��ǥ��
	int		CountX;
	int		CountY;
	int		CountZ;

	CollisionSectionInfo() :
		CountX(1),
		CountY(1),
		CountZ(1),
		SectionSize{ 1000.f, 1000.f, 1.f },
		SectionTotalSize{ 1000.f, 1000.f, 1.f },
		Min{ -500.f, -500.f, -0.5f },
		Max{ 500.f, 500.f, 0.5f }
	{
	}
};


class CSceneCollision
{
	friend class CScene;

private:
	CSceneCollision();
	~CSceneCollision();

private:
	class CScene* m_Scene;

private:
	CollisionSectionInfo* m_Section;
	// ��� ColliderComponent �� ��Ƴ���
	std::list<CSharedPtr<class CColliderComponent>>	m_ColliderList;
	// ���� ������ ���콺�� �浹�� �浹ü
	CSharedPtr<class CColliderComponent> m_MouseCollision;
	bool m_WidgetClick;


public:
	class CColliderComponent* GetMouseCollision()	const
	{
		return m_MouseCollision;
	}

	void DeleteCollider(class CColliderComponent* Collider);

public:
	void Start();
	bool Init();
	bool CollisionWidget(float DeltaTime);
	void Collision(float DeltaTime);

protected:
	void CollisionMouse(float DeltaTime);

public:
	void SetSectionSize(const Vector3& Size);
	void SetSectionSize(float x, float y, float z = 1.f);
	void SetSectionCenter(const Vector3& Center);
	void SetSectionCenter(float x, float y, float z);
	void SetSectionMin(const Vector3& Min);
	void SetSectionMin(float x, float y, float z);
	void SetSectionMax(const Vector3& Max);
	void SetSectionMax(float x, float y, float z);
	void SetSectionCount(int CountX, int CountY, int CountZ = 1);
	void CreateSection();
	void Clear();
	void AddCollider(class CColliderComponent* Collider);

private:
	void CheckColliderSection();

};

