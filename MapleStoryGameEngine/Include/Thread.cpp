
#include "Thread.h"

CThread::CThread() :
	m_Thread(0),
	m_StartEvent(0)
{
}

CThread::~CThread()
{
	if (m_StartEvent)
	{
		CloseHandle(m_StartEvent);
		m_StartEvent = 0;
	}
}

bool CThread::Init()
{
	// CreateEvent의 1번째 인자는 자식 프로세스에게 상속여부
	// 2번째 인자는 False로 하면 자동 reset mode.
	// 여기서 reset은 이벤트 객체가 signaled->non-signaled로 되는걸 의미한다.
	// WaitForSingleObject를 빠져나와서 signaled 상태이면 수동 reset mode이고, non-signaled 상태이면 자동 reset mode이다
	// 3번째 인자는 False로 하면 초기 상태를 non-signaled 상태로 생성
	// 4번째 인자는 이벤트 이름
	m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// 4번째 인자는 ThreadFunction에 넘겨줄 인자
	// 5번째 인자인 initflag를 0으로 설정 하면 쓰레드 생성과 동시에 즉시 실행 할 수 있다
	// 마지막 인자에는 ThreadID를 받고 싶다면 unsigned int* 타입의 변수를 넣어두면 된다
	m_Thread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunction,
		this, 0, nullptr);

	return true;
}

void CThread::Start()
{
	// 이벤트에게 non-signal->signal 상태로 바꿔줘서 쓰레드가 blocking에서 빠져나오게 한다
	// CThread::ThreadFunction에서 WaitStartEvent로 인해 Blocking되어 있는 상태에서, 다른 쓰레드가 Start호출을 해줘서
	// 여기서 SetEvent를 해줄 수 있게 돼서 Blocking에서 빠져나오고 아랫줄에 있는 Thread->Run();을 실행할 수 있게된다
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	DWORD Count = 0;

	do
	{
		// SuspendThread와 ResumeThread는 내부적으로 Count를 유지해서 
		// Suspend한 횟수만큼 Resume을 해줘야 다시 Resume한다 
		// SuspendThread를 호출할때마다 이 Count를 증가시킨다
		// return값은 이전에 Suspend한 횟수를 return한다
		Count = SuspendThread(m_Thread);
	} while (Count <= 0);
}

void CThread::Resume()
{
	DWORD	Count = 0;
	do
	{
		// ResumeThread function decrements and returns a thread's suspend count.
		Count = ResumeThread(m_Thread);
	} while (Count >= 0);
}

void CThread::WaitStartEvent()
{
	WaitForSingleObject(m_StartEvent, INFINITE);
}

unsigned int __stdcall CThread::ThreadFunction(void* Arg)
{
	CThread* Thread = (CThread*)Arg;

	// 쓰레드 생성되자마자 여기로 들어와봤자 다른 쓰레드가 CThread::Start 호출해주기 전까지는 여기서 blocking상태로 대기
	Thread->WaitStartEvent();

	Thread->Run();

	return 0;
}