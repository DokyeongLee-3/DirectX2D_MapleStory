#pragma once
#include "ConstantBufferBase.h"

class CTransformConstantBuffer :
    public CConstantBufferBase
{
public:
	CTransformConstantBuffer();
	CTransformConstantBuffer(const CTransformConstantBuffer& Buffer);
	virtual ~CTransformConstantBuffer();

protected:
	// GameInfo에 선언한, ShaderInfo.fx에 cbuffer Transform와
	// 대응되는 구조체
	TransformCBuffer m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CTransformConstantBuffer* Clone();

public:
	void SetWorldMatrix(const Matrix& matWorld)
	{
		m_BufferData.matWorld = matWorld;
	}

	void SetViewMatrix(const Matrix& matView)
	{
		m_BufferData.matView = matView;
	}

	void SetProjMatrix(const Matrix& matProj)
	{
		m_BufferData.matProj = matProj;
	}

	void SetPivot(const Vector3& Pivot)
	{
		m_BufferData.Pivot = Pivot;
	}

	void SetMeshSize(const Vector3& Size)
	{
		m_BufferData.MeshSize = Size;
	}
};

