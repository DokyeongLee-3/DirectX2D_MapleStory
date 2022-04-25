#pragma once

#include "../Component/SpriteComponent.h"

template <typename T>
class CMemoryBlock
{
private:
	void* m_BlockStart;
	// ��ü ������
	size_t m_ArenaSize;
	// �� ó�� Allocate�� �̿��ؼ� �Ҵ��� ������� ��� �Ҵ��ؾ� �ϴµ�(���� �޸𸮺�� ����̹Ƿ�)
	// ���߿� �� ū ����� Allocate�Ϸ� �Ѵٸ� bad_alloc ����ó��
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
		// arena_size �Ѱ� Allocate�Ϸ��� �ϸ� ���� ������� ����
		// ������ CMemoryBlock::Allocate�� �ҷȴµ� �� �Ҹ���, m_FreeBlock�� nullptr�̴� -> arena_size��ŭ ��� ����
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
			// �̹� �Ҵ�� �޸� ������ TŸ�� Ŭ���� ������ ���� ȣ��
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

