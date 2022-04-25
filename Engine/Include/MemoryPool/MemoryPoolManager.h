#pragma once

#include <memory>
#include <iostream>
#include "MemoryBlock.h"

template <typename T>
class CMemoryPoolManager
{
public:
	std::unique_ptr<CMemoryBlock<T>> MemoryBlockHandle;

	// CMemoryBlock::Allocate로부터 할당받은 메모리 chunk(arena_size의 크기)
	T* m_MemoryChunk;

	size_t m_ArenaSize;
	T* m_FreeBlock;

public:
	CMemoryPoolManager(size_t arena_size)	:
		m_ArenaSize(arena_size)
	{
		MemoryBlockHandle = std::make_unique<CMemoryBlock<T>>(arena_size);
	}

	~CMemoryPoolManager()
	{
		// MemoryPoolManager로 부터 m_MemoryChunk(마지막으로 반납된 메모리 블럭)을 얻고
		// 그 m_MemoryChunk 해제 -> Next -> 해제 -> Next 반복해야함 
		int Count = MemoryBlockHandle->GetCount();
		size_t UnitSize = sizeof(T);

		for (int i = 0; i < Count; ++i)
		{
			m_FreeBlock = reinterpret_cast<T*>(m_MemoryChunk);
			auto Next = m_FreeBlock->GetNext();

			//m_FreeBlock->~T();
			free(m_FreeBlock);

			m_FreeBlock = Next;
		}
	}

	T* Allocate(size_t size)
	{
		try
		{
			if (size < 0)
			{
				throw std::bad_alloc();
			}

			if (!m_MemoryChunk)
			{
				m_MemoryChunk = reinterpret_cast<T*>(MemoryBlockHandle->Allocate(size));
			}

			// CMemoryBlock::Allocate가 호출돼서 여기로 점프하는건 맨처음 Allocate 호출된 경우이거나(정상)
			// 처음 Allocate한거 보다 더 큰 사이즈를 Allocate하려거나(고정 메모리 블록 방식이므로 예외처리)
			if (MemoryBlockHandle->GetUnitSize() < size)
			{
				throw std::bad_alloc();
			}
		}

		catch (const std::bad_alloc& bad)
		{
			std::cout << "bad allocation" << std::endl;
			exit(1);
		}

		auto ret = m_MemoryChunk;
		m_MemoryChunk = m_MemoryChunk->GetNext();


		return ret;
	}

	void Deallocate(void* mem)
	{
		if (mem)
		{
			auto p = m_MemoryChunk;

			m_MemoryChunk = reinterpret_cast<T*>(mem);
			m_MemoryChunk->SetNext(p);
		}
	}

public:
	T* GetNextChunk()	const
	{
		return m_MemoryChunk;
	}

};