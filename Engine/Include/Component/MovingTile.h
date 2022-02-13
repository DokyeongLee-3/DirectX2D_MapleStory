#pragma once

#include "../Engine.h"

class CMovingTile
{
	friend class CMovingTileMapComponent;

private:
	CMovingTile();
	CMovingTile(const CMovingTile& tile);
	~CMovingTile();

private:
	class CMovingTileMapComponent* m_Owner;
	Vector2		m_Pos;			// ¡¬ «œ¥‹ ±‚¡ÿ.
	Vector2		m_Size;
	Vector2		m_FrameStart;
	Vector2		m_FrameEnd;
	float		m_Opacity;
	Matrix		m_matWorld;
	Vector2		m_Speed;

public:
	float GetOpacity()	const
	{
		return m_Opacity;
	}

	const Vector2& GetSize()	const
	{
		return m_Size;
	}

	const Vector2& GetPos()	const
	{
		return m_Pos;
	}

	const Matrix& GetWorldMatrix()	const
	{
		return m_matWorld;
	}

	Vector2 GetFrameStart()	const
	{
		return m_FrameStart;
	}

	Vector2 GetFrameEnd()	const
	{
		return m_FrameEnd;
	}

	bool GetRender()	const
	{
		return m_FrameStart.x != 0.f || m_FrameStart.y != 0.f || m_FrameEnd.x != 0.f || m_FrameEnd.y != 0.f;
	}

public:
	void SetOwner(class CMovingTileMapComponent* Owner);

	void SetSpeed(const Vector2& Speed)
	{
		m_Speed = Speed;
	}

	void SetOpacity(float Opacity)
	{
		m_Opacity = Opacity;
	}

	void SetFrameStart(const Vector2& Frame)
	{
		m_FrameStart = Frame;
	}

	void SetFrameStart(float x, float y)
	{
		m_FrameStart = Vector2(x, y);
	}

	void SetFrameEnd(const Vector2& Frame)
	{
		m_FrameEnd = Frame;
	}

	void SetFrameEnd(float x, float y)
	{
		m_FrameEnd = Vector2(x, y);
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

public:
	void Start();
	void Update(float DeltaTime);
};

