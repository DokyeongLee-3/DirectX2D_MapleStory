#pragma once

#include "ConstantBufferBase.h"

class CMovingTileMapConstantBuffer :
    public CConstantBufferBase
{
public:
	CMovingTileMapConstantBuffer();
	CMovingTileMapConstantBuffer(const CMovingTileMapConstantBuffer& Buffer);
	virtual ~CMovingTileMapConstantBuffer();

protected:
	MovingTileMapCBuffer	m_BufferData;

public:
	virtual bool Init();
	virtual void UpdateCBuffer();
	virtual CMovingTileMapConstantBuffer* Clone();

public:
	void SetImageSize(const Vector2& Size)
	{
		m_BufferData.ImageSize = Size;
	}
};

