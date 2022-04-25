#pragma once
//#include "..\BlockUnit\CBlockUnit.h"

class CMemoryBlock
{
private:
	void* m_BlockStart;
	void* m_ArenaStart;
	// ��ü ������
	size_t m_ArenaSize;
	// �� ó�� Allocate�� �̿��ؼ� �Ҵ��� ������� ��� �Ҵ��ؾ� �ϴµ�(���� �޸𸮺�� ����̹Ƿ�)
	// ���߿� �� ū ����� Allocate�Ϸ� �Ѵٸ� bad_alloc ����ó��
	size_t m_UnitSize;
	//CBlockUnit* m_FreeBlock;
	size_t m_RemainingSize;

public:
	CMemoryBlock(size_t arena_size);
	~CMemoryBlock();

public:
	// �̹� arena_size��ŭ �Ҵ��س��� MemoryBlock�� size��ŭ���� �ɰ��� �Լ�
	//void* Allocate(size_t size);
	void Deallocate(void* mem);

public:
	int GetCount();
	size_t GetArenaSize();
	size_t GetUnitSize();


	// ���� �� �� �ִ� �޸� Block�� return
	void* GetBlockStart(int Size);

	
};

