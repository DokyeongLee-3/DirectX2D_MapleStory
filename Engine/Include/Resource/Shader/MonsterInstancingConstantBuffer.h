#pragma once

#include "ConstantBufferBase.h"

class CMonsterInstancingConstantBuffer :
    public CConstantBufferBase
{
public:
	CMonsterInstancingConstantBuffer();
	CMonsterInstancingConstantBuffer(const CMonsterInstancingConstantBuffer& Buffer);
	virtual ~CMonsterInstancingConstantBuffer();

protected:
	MonsterCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CMonsterInstancingConstantBuffer* Clone();


public:
	unsigned int GetStartSpawnEnable()	const
	{
		return m_BufferData.MonsterSpawnEnable;
	}

	unsigned int GetMonsterSpawnCountMax()	const
	{
		return m_BufferData.MonsterSpawnCountMax;
	}

public:
	void SetSpawnCountMax(unsigned int Max)
	{
		m_BufferData.MonsterSpawnCountMax = Max;
	}
};

