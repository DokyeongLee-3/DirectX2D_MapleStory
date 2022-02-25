#pragma once

#include "../Thread.h"
#include "../ThreadQueue.h"
#include "../Component/SceneComponent.h"

class CNavigationThread :
	public CThread
{
	friend class CNavigationManager;

public:
	CNavigationThread();
	virtual ~CNavigationThread();

private:
	class CNavigationManager*			m_NavManager;
	CThreadQueue<NavWorkData>			m_WorkQueue;
	// 실제로 길찾기를 해주는 객체
	class CNavigation*					m_Navigation;
	HANDLE				m_ExitEvent;

public:
	int GetWorkCount()
	{
		return m_WorkQueue.size();
	}

	void CreateNavigationNode(class CTileMapComponent* TileMap);

public:
	virtual void Run();

public:
	template <typename T, typename ComponentType>
	void AddWork(T* Obj, void(T::* Func)(const std::list<Vector3>&),
		ComponentType* OwnerComponent, const Vector3& End)
	{
		NavWorkData	Data;
		Data.Callback = std::bind(Func, Obj, std::placeholders::_1);
		//Data.Start = Start;
		Data.OwnerComponent = (CSceneComponent*)OwnerComponent;
		Data.End = End;

		m_WorkQueue.push(Data);
	}
};

