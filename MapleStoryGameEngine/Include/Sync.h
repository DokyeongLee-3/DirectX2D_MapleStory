#pragma once

#include "GameInfo.h"

// ���� ���, CRITICAL_SECTION crt; CSync sync(&crt);
// �̷��� ���� CSync�� ���������� ���� Critical Section���� ������ �κ��� 
// �Ҹ��� ȣ��� ���ÿ� unlock�ȴ�

class CSync
{
public:
	CSync(CRITICAL_SECTION* Crt)
	{
		m_Crt = Crt;
		EnterCriticalSection(m_Crt);
	}

	~CSync()
	{
		LeaveCriticalSection(m_Crt);
	}

private:
	CRITICAL_SECTION* m_Crt;
};

