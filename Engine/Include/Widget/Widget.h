#pragma once

#include "../Ref.h"
#include "../Resource/Texture/Texture.h"

struct WidgetImageInfo
{
	CSharedPtr<CTexture>	Texture;
	Vector4			Tint;
	std::vector<AnimationFrameData> vecFrameData;

	int		Frame;		// 현재 애니메이션 프레임
	float	Time;			// 애니메이션 동작 시간
	float	FrameTime;	// 1프레임당 시간
	float	PlayTime;
	float	PlayScale;		// 재생 비율

	WidgetImageInfo()
	{
		Tint = Vector4::White;
		Frame = 0;
		Time = 0.f;
		FrameTime = 0.f;
		PlayTime = 1.f;
		PlayScale = 1.f;
	}

	WidgetImageInfo(const WidgetImageInfo& Info)
	{
		Texture = Info.Texture;
		Tint = Info.Tint;
		Frame = Info.Frame;
		Time = Info.Time;
		FrameTime = Info.FrameTime;
		PlayTime = Info.PlayTime;
		PlayScale = Info.PlayScale;
		
		size_t Size = Info.vecFrameData.size();

		for (size_t i = 0; i < Size; ++i)
		{
			vecFrameData.push_back(Info.vecFrameData[i]);
		}
	}
};

// 위젯의 위치는 좌하단 기준
class CWidget :
	public CRef
{
	friend class CWidgetWindow;

protected:
	CWidget();
	CWidget(const CWidget& widget);
	virtual ~CWidget();

protected:
	class CWidgetWindow* m_Owner;
	int     m_ZOrder;
	Vector2	m_Pos;
	Vector2	m_RenderPos;
	Vector2	m_Size;
	float	m_Angle;
	float	m_Opacity;
	bool	m_Start;
	Vector4	m_Tint;
	bool	m_MouseHovered;
	bool	m_CollisionMouseEnable;
	class CWidgetConstantBuffer* m_CBuffer;
	CSharedPtr<class CShader>	m_Shader;
	CSharedPtr<class CMesh>		m_Mesh;

	bool m_RenderStart;


public:
	void SetRenderStart(bool Start)
	{
		m_RenderStart = Start;
	}

	bool GetRenderStart()	const
	{
		return m_RenderStart;
	}

	virtual void Enable(bool bEnable)
	{
		CRef::Enable(bEnable);
	}

	class CWidgetWindow* GetOwner()	const
	{
		return m_Owner;
	}

	Vector2 GetWidgetPos()	const
	{
		return m_Pos;
	}

	Vector2 GetWidgetSize()	const
	{
		return m_Size;
	}

	int GetZOrder()	const
	{
		return m_ZOrder;
	}

	float GetAngle()	const
	{
		return m_Angle;
	}

	float GetOpacity()	const
	{
		return m_Opacity;
	}

public:
	void SetOpacity(float Opacity)
	{
		m_Opacity = Opacity;
	}

	void SetZOrder(int ZOrder)
	{
		m_ZOrder = ZOrder;
	}

	void SetPos(const Vector2& Pos)
	{
		m_Pos = Pos;
	}

	void SetPos(float x, float y)
	{
		m_Pos = Vector2(x, y);
	}

	virtual void SetSize(const Vector2& Size)
	{
		m_Size = Size;
	}

	virtual void SetSize(float x, float y)
	{
		m_Size = Vector2(x, y);
	}

	void SetOwner(class CWidgetWindow* Owner)
	{
		m_Owner = Owner;
	}

	void SetAngle(float Angle)
	{
		m_Angle = Angle;
	}

	void SetMouseCollisionEnable(bool Enable)
	{
		m_CollisionMouseEnable = Enable;
	}

	void SetShader(const std::string& Name);
	void SetUseTexture(bool Use);

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual bool CollisionMouse(const Vector2& MousePos);
	virtual CWidget* Clone(); 
};

