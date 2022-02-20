
#include "NavigationThread.h"
#include "Navigation.h"
#include "NavigationManager.h"

CNavigationThread::CNavigationThread()
{
	SetLoop(true);

	m_Navigation = new CNavigation;
}

CNavigationThread::~CNavigationThread()
{
	SAFE_DELETE(m_Navigation);
}

void CNavigationThread::CreateNavigationNode(CTileMapComponent* TileMap)
{
	m_Navigation->CreateNavigationNode(TileMap);
}

void CNavigationThread::Run()
{
	while (m_Loop)
	{
		if (!m_WorkQueue.empty())
		{
			NavWorkData Work = m_WorkQueue.front();
			m_WorkQueue.pop();

			NavResultData	Result;

			m_Navigation->FindPath(Work.Start, Work.End, Result.vecPath);

			Result.Callback = Work.Callback;

			m_NavManager->AddNavResult(Result);
		}
	}
}
