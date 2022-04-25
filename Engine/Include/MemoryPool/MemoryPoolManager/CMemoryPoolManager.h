#pragma once
#include <memory>
#include <iostream>
//#include "..\BlockUnit\CBlockUnit.h"
#include "..\MemoryBlock\CMemoryBlock.h"
#include "../../GameInfo.h"

class CMemoryPoolManager
{
	friend class CResourceManager;

private:
	std::unique_ptr<CMemoryBlock> MemoryBlockHandle;

	// CMemoryBlock::Allocate�κ��� �Ҵ���� �޸� chunk(arena_size�� ũ��)
	//CBlockUnit* m_MemoryChunk;

	size_t m_ArenaSize;

	int m_RemainingSize;
	

public:
	bool Init(size_t arena_size);
	void* Allocate(size_t size);
	void Deallocate(void* mem);


public:
	void* RequestMemoryChunk(size_t Size);

	// ������ ����� �޸� �ּ�
	//CBlockUnit* GetNextChunk()	const
	//{
	//	return m_MemoryChunk;
	//}

	int GetRemainingSize()	const
	{
		return m_RemainingSize;
	}
};

