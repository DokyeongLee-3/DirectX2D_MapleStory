#pragma once

#include "ConstantBufferBase.h"

// 이 클래스자체는 CAnimationManager에 들어가있고
// 이 클래스의 부모가 갖고 있는 CConstantBuffer타입의
// 클래스는 CShaderManager에 있다
class CAnimation2DConstantBuffer :
    public CConstantBufferBase
{
public:
	CAnimation2DConstantBuffer();
	CAnimation2DConstantBuffer(const CAnimation2DConstantBuffer& Buffer);
	virtual ~CAnimation2DConstantBuffer();

protected:
	// 이 멤버는 실제 상수버퍼와 대응되는 구조체일뿐이고
	// CConstantBufferBase에서 들고 있는 m_Buffer는
	// 실제 버퍼타입(ID3D11Buffer)을 멤버로 갖고 있는
	// CConstantBuffer 클래스에 대한 포인터이다.
	Animation2DCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CAnimation2DConstantBuffer* Clone();

public:
	void SetAnimation2DType(Image_Type Type)
	{
		m_BufferData.Animation2DType = (int)Type;
	}

	void SetStartUV(const Vector2& StartUV)
	{
		m_BufferData.Animation2DStartUV = StartUV;
	}

	void SetEndUV(const Vector2& EndUV)
	{
		m_BufferData.Animation2DEndUV = EndUV;
	}
};

