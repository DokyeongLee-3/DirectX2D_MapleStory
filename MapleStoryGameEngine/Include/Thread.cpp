
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
	// CreateEvent�� 1��° ���ڴ� �ڽ� ���μ������� ��ӿ���
	// 2��° ���ڴ� False�� �ϸ� �ڵ� reset mode.
	// ���⼭ reset�� �̺�Ʈ ��ü�� signaled->non-signaled�� �Ǵ°� �ǹ��Ѵ�.
	// WaitForSingleObject�� �������ͼ� signaled �����̸� ���� reset mode�̰�, non-signaled �����̸� �ڵ� reset mode�̴�
	// 3��° ���ڴ� False�� �ϸ� �ʱ� ���¸� non-signaled ���·� ����
	// 4��° ���ڴ� �̺�Ʈ �̸�
	m_StartEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);

	// 4��° ���ڴ� ThreadFunction�� �Ѱ��� ����
	// 5��° ������ initflag�� 0���� ���� �ϸ� ������ ������ ���ÿ� ��� ���� �� �� �ִ�
	// ������ ���ڿ��� ThreadID�� �ް� �ʹٸ� unsigned int* Ÿ���� ������ �־�θ� �ȴ�
	m_Thread = (HANDLE)_beginthreadex(nullptr, 0, CThread::ThreadFunction,
		this, 0, nullptr);

	return true;
}

void CThread::Start()
{
	// �̺�Ʈ���� non-signal->signal ���·� �ٲ��༭ �����尡 blocking���� ���������� �Ѵ�
	// CThread::ThreadFunction���� WaitStartEvent�� ���� Blocking�Ǿ� �ִ� ���¿���, �ٸ� �����尡 Startȣ���� ���༭
	// ���⼭ SetEvent�� ���� �� �ְ� �ż� Blocking���� ���������� �Ʒ��ٿ� �ִ� Thread->Run();�� ������ �� �ְԵȴ�
	SetEvent(m_StartEvent);
}

void CThread::Pause()
{
	DWORD Count = 0;

	do
	{
		// SuspendThread�� ResumeThread�� ���������� Count�� �����ؼ� 
		// Suspend�� Ƚ����ŭ Resume�� ����� �ٽ� Resume�Ѵ� 
		// SuspendThread�� ȣ���Ҷ����� �� Count�� ������Ų��
		// return���� ������ Suspend�� Ƚ���� return�Ѵ�
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

	// ������ �������ڸ��� ����� ���ͺ��� �ٸ� �����尡 CThread::Start ȣ�����ֱ� �������� ���⼭ blocking���·� ���
	Thread->WaitStartEvent();

	Thread->Run();

	return 0;
}