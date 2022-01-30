
#include "MonsterInstancingConstantBuffer.h"
#include "ConstantBuffer.h"

CMonsterInstancingConstantBuffer::CMonsterInstancingConstantBuffer()	:
	m_BufferData{}
{
	m_BufferData.MonsterSpawnCountMax = 20;
}

CMonsterInstancingConstantBuffer::CMonsterInstancingConstantBuffer(const CMonsterInstancingConstantBuffer& Buffer)	:
	CConstantBufferBase(Buffer)
{
	m_BufferData = Buffer.m_BufferData;
}

CMonsterInstancingConstantBuffer::~CMonsterInstancingConstantBuffer()
{
}

bool CMonsterInstancingConstantBuffer::Init()
{
	m_BufferData.MonsterSpawnCountMax = 20;

	SetConstantBuffer("MonsterInstancingCBuffer");

	return true;
}

void CMonsterInstancingConstantBuffer::UpdateCBuffer()
{
	m_Buffer->UpdateBuffer(&m_BufferData);
}

CMonsterInstancingConstantBuffer* CMonsterInstancingConstantBuffer::Clone()
{
	return new CMonsterInstancingConstantBuffer(*this);
}
