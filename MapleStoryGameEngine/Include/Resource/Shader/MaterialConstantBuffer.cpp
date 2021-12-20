
#include "MaterialConstantBuffer.h"
#include "ConstantBuffer.h"

CMaterialConstantBuffer::CMaterialConstantBuffer() :
	m_BufferData{}
{
}

CMaterialConstantBuffer::CMaterialConstantBuffer(const CMaterialConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CMaterialConstantBuffer::~CMaterialConstantBuffer()
{
}

bool CMaterialConstantBuffer::Init()
{
	SetConstantBuffer("MaterialCBuffer");

	return true;
}

void CMaterialConstantBuffer::UpdateCBuffer()
{
	// 벡터는 행렬과 다르게 Transpose 해주지 않고 넘겨도된다
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CMaterialConstantBuffer* CMaterialConstantBuffer::Clone()
{
	return new CMaterialConstantBuffer(*this);
}
