
#include "Viewport.h"
#include "../Input.h"

CViewport::CViewport()
{
}

CViewport::~CViewport()
{
}

void CViewport::Start()
{
	auto	iter = m_WindowList.begin();
	auto	iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Start();
	}
}

bool CViewport::Init()
{
	return true;
}

void CViewport::Update(float DeltaTime)
{
	auto	iter = m_WindowList.begin();
	auto	iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->Update(DeltaTime);
		++iter;
	}
}

void CViewport::PostUpdate(float DeltaTime)
{
	auto	iter = m_WindowList.begin();
	auto	iterEnd = m_WindowList.end();

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		else if (!(*iter)->IsEnable())
		{
			++iter;
			continue;
		}

		(*iter)->PostUpdate(DeltaTime);
		++iter;
	}
}

bool CViewport::CollisionMouse()
{
	Vector2 MousePos = CInput::GetInst()->GetMousePos();

	// 출력 우선순위가 높은 Window부터 충돌처리를 시작한다.
	if (m_WindowList.size() >= 2)
		m_WindowList.sort(CViewport::SortWindow);

	auto	iter = m_WindowList.begin();
	auto	iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}

		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		if ((*iter)->CollisionMouse(MousePos))
			return true;
	}

	// 마우스로 UI창을 닫거나 할 수 있으니까 위에서 UI와 마우스 충돌 돌려보고
	// 여기서 Active = false인건 Destroy
	//for (; iter != iterEnd;)
	//{
	//	if (!(*iter)->IsActive())
	//	{
	//		iter = m_WindowList.erase(iter);
	//		iterEnd = m_WindowList.end();
	//		continue;
	//	}

	//	++iter;
	//}

	return false;
}

void CViewport::Collision()
{
	auto	iter = m_WindowList.begin();
	auto	iterEnd = m_WindowList.end();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
		{
			continue;
		}
	}

	for (; iter != iterEnd;)
	{
		if (!(*iter)->IsActive())
		{
			iter = m_WindowList.erase(iter);
			iterEnd = m_WindowList.end();
			continue;
		}

		++iter;
	}
}

void CViewport::Render()
{
	// Window가 ZOrder 내림차순으로 정렬되어 있으니
	// 역순으로 출력
	auto	iter = m_WindowList.rbegin();
	auto	iterEnd = m_WindowList.rend();

	for (; iter != iterEnd; ++iter)
	{
		if (!(*iter)->IsEnable())
			continue;

		if (!(*iter)->GetVisibility())
			continue;

		(*iter)->Render();
	}
}

bool CViewport::SortWindow(CSharedPtr<CWidgetWindow> Src, CSharedPtr<CWidgetWindow> Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}

CWidgetWindow* CViewport::FindTopMostWindow()
{
	if (m_WindowList.size() > 2)
		m_WindowList.sort(CViewport::SortWindow);

	auto iter = m_WindowList.begin();

	while (!(*iter)->IsEnable())
		++iter;

	return (*iter);
}
