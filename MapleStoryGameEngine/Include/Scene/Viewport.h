#pragma once

#include "../GameInfo.h"
#include "../Widget/WidgetWindow.h"

class CViewport
{
	friend class CScene;

protected:
	CViewport();
	~CViewport();

private:
	class CScene* m_Scene;

public:
	class CScene* GetScene()	const
	{
		return m_Scene;
	}

	void SetScene(CScene* Scene)
	{
		m_Scene = Scene;
	}

private:
	std::list<CSharedPtr<CWidgetWindow>>	m_WindowList;

public:
	void Start();
	bool Init();
	void Update(float DeltaTime);
	void PostUpdate(float DeltaTime);
	bool CollisionMouse();
	void Collision();
	void Render();

private:
	static bool SortWindow(CSharedPtr<CWidgetWindow> Src, CSharedPtr<CWidgetWindow> Dest);

public:
	// ZOrder가 가장 높은 Window를 찾아서 리턴
	CWidgetWindow* FindTopmostWindow();
	void AddWindow(CWidgetWindow* Window);
	int GetTopmostWindowZOrder();

public:
	template <typename T>
	CWidgetWindow* FindWidgetWindow(const std::string& Name)
	{
		auto	iter = m_WindowList.begin();
		auto	iterEnd = m_WindowList.end();

		for (; iter != iterEnd; ++iter)
		{
			if ((*iter)->GetName() == Name)
				return *iter;
		}

		return nullptr;
	}

	template <typename T>
	T* CreateWidgetWindow(const std::string& Name)
	{
		T* Window = new T;

		Window->SetName(Name);
		Window->m_Viewport = this;

		if (!Window->Init())
		{
			SAFE_RELEASE(Window);
			return nullptr;
		}

		m_WindowList.push_back((CWidgetWindow*)Window);


		return Window;
	}
};

