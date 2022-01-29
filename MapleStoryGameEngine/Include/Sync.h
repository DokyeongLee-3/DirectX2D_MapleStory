#pragma once

#include "GameInfo.h"

// 예를 들어, CRITICAL_SECTION crt; CSync sync(&crt);
// 이렇게 쓰면 CSync를 지역변수로 쓰면 Critical Section으로 지정된 부분은 
// 소멸자 호출과 동시에 unlock된다

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

