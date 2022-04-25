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

	// CMemoryBlock::Allocate로부터 할당받은 메모리 chunk(arena_size의 크기)
	//CBlockUnit* m_MemoryChunk;

	size_t m_ArenaSize;

	int m_RemainingSize;
	

public:
	bool Init(size_t arena_size);
	void* Allocate(size_t size);
	void Deallocate(void* mem);


public:
	void* RequestMemoryChunk(size_t Size);

	// 다음에 사용할 메모리 주소
	//CBlockUnit* GetNextChunk()	const
	//{
	//	return m_MemoryChunk;
	//}

	int GetRemainingSize()	const
	{
		return m_RemainingSize;
	}
};

