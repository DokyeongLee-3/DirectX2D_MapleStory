#pragma once

#include "Thread.h"
#include "ThreadQueue.h"
#include "../Client.h"

class CLoadingThread :
	public CThread
{
public:
	CLoadingThread();
	virtual ~CLoadingThread();

private:
	CThreadQueue<LoadingMessage>	m_Queue;
	// Run에서 어떤 Scene을 초기화할지 갖고 있는다
	ThreadLoadingScene				m_LoadingScene;

public:
	ThreadLoadingScene	GetLoadingScene()	const
	{
		return m_LoadingScene;
	}

	void SetLoadingScene(ThreadLoadingScene Scene)
	{
		m_LoadingScene = Scene;
	}

	CThreadQueue<LoadingMessage>* GetLoadingQueue()
	{
		return &m_Queue;
	}

	void AddMessage(bool Complete, float Percent)
	{
		LoadingMessage	Msg;

		Msg.Complete = Complete;
		Msg.Percent = Percent;

		m_Queue.push(Msg);
	}

public:
	bool Init();
	virtual void Run();
};

