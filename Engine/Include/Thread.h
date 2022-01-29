#pragma once

#include "GameInfo.h"

class CThread
{
protected:
	CThread();
	virtual ~CThread() = 0;

private:
	std::string m_Name;
	HANDLE		m_Thread;
	HANDLE		m_StartEvent;

public:
	bool Init();
	virtual void Run() = 0;

public:
	void Start();
	void Pause();
	void Resume();

private:
	void WaitStartEvent();
	// 스레드 함수 형태는 무조건 void*를 인자로 받고 리턴 타입은 unsigned int
	// 호출규약은 stdcall방식이어야 한다
	static unsigned int __stdcall ThreadFunction(void* Arg);

public:
	template <typename T>
	static T* CreateThread(const std::string& Name)
	{
		T* Thread = new T;

		Thread->m_Name = Name;

		if (!Thread->Init())
		{
			SAFE_DELETE(Thread);
			return nullptr;
		}

		return Thread;
	}
};

