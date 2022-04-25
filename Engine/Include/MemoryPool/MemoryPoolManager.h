#pragma once

#include <memory>
#include <iostream>
#include "MemoryBlock.h"

template <typename T>
class CMemoryPoolManager
{
public:
	std::unique_ptr<CMemoryBlock<T>> MemoryBlockHandle;

	// CMemoryBlock::Allocate�κ��� �Ҵ���� �޸� chunk(arena_size�� ũ��)
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
		// MemoryPoolManager�� ���� m_MemoryChunk(���������� �ݳ��� �޸� ��)�� ���
		// �� m_MemoryChunk ���� -> Next -> ���� -> Next �ݺ��ؾ��� 
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

			// CMemoryBlock::Allocate�� ȣ��ż� ����� �����ϴ°� ��ó�� Allocate ȣ��� ����̰ų�(����)
			// ó�� Allocate�Ѱ� ���� �� ū ����� Allocate�Ϸ��ų�(���� �޸� ��� ����̹Ƿ� ����ó��)
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