#pragma once

#include "../Component/SpriteComponent.h"

template <typename T>
class CMemoryBlock
{
private:
	void* m_BlockStart;
	// 전체 사이즈
	size_t m_ArenaSize;
	// 맨 처음 Allocate를 이용해서 할당한 사이즈로 계속 할당해야 하는데(고정 메모리블록 방식이므로)
	// 나중에 더 큰 사이즈를 Allocate하려 한다면 bad_alloc 예외처리
	size_t m_UnitSize;
	T* m_FreeBlock;


public:
	CMemoryBlock(size_t arena_size) :
		m_UnitSize(0),
		m_ArenaSize(arena_size)
	{
		
		//char* p = new char[arena_size];
		//m_BlockStart = p;
		//m_FreeBlock = reinterpret_cast<T*>(p);
		//

		char* p = (char*)malloc(arena_size);
		m_BlockStart = p;
		m_FreeBlock = reinterpret_cast<T*>(p);
	}

	~CMemoryBlock()
	{
		//char* p = reinterpret_cast<char*>(m_BlockStart);
		//delete [] p;
	}

public:
	void* Allocate(size_t size)
	{
		// Is it required?
		// size = std::max(size, sizeof(void*));

		if (m_UnitSize == 0)
		{
			m_UnitSize = size;
		}
		// arena_size 넘게 Allocate하려고 하면 여기 블록으로 진입
		// 이전에 CMemoryBlock::Allocate가 불렸는데 또 불리고, m_FreeBlock이 nullptr이다 -> arena_size만큼 모두 소진
		else
		{
			if (m_FreeBlock == nullptr)
			{
				throw std::bad_alloc();
			}
		}

		int count = GetCount();

		auto ret = m_FreeBlock;

		for (int i = 0; i < count; ++i)
		{
			// 이미 할당된 메모리 공간에 T타입 클래스 생성자 강제 호출
			new(m_FreeBlock) T();

			auto p = reinterpret_cast<char*>(m_FreeBlock) + size;
			if (i != count - 1)
			{
				m_FreeBlock->SetNext(p);
			}

			else
			{
				m_FreeBlock->SetNext(nullptr);
			}

			m_FreeBlock = m_FreeBlock->GetNext();
		}

		return ret;
	}

	void Deallocate(T* mem)
	{
		delete [] mem;
	}

public:
	int GetCount()
	{
		return m_ArenaSize / m_UnitSize;
	}

	size_t GetArenaSize()
	{
		return m_ArenaSize;
	}

	size_t GetUnitSize()
	{
		return m_UnitSize;
	}

	void* GetBlockStart()
	{
		return m_BlockStart;
	}

};

