
#include "MovingTileMapConstantBuffer.h"
#include "ConstantBuffer.h"

CMovingTileMapConstantBuffer::CMovingTileMapConstantBuffer() :
	m_BufferData{}
{
}

CMovingTileMapConstantBuffer::CMovingTileMapConstantBuffer(const CMovingTileMapConstantBuffer& Buffer) :
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CMovingTileMapConstantBuffer::~CMovingTileMapConstantBuffer()
{
}

bool CMovingTileMapConstantBuffer::Init()
{
	SetConstantBuffer("MovingTileMapCBuffer");

	return true;
}

void CMovingTileMapConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CMovingTileMapConstantBuffer* CMovingTileMapConstantBuffer::Clone()
{
	return new CMovingTileMapConstantBuffer(*this);
}
