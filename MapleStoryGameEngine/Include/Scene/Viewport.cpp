
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

	// ��� �켱������ ���� Window���� �浹ó���� �����Ѵ�.
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

		if ((*iter)->CollisionMouse(MousePos))
			return true;
	}

	// ���콺�� UIâ�� �ݰų� �� �� �����ϱ� ������ UI�� ���콺 �浹 ��������
	// ���⼭ Active = false�ΰ� Destroy
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
	// Window�� ZOrder ������������ ���ĵǾ� ������
	// �������� ���
	auto	iter = m_WindowList.rbegin();
	auto	iterEnd = m_WindowList.rend();

	for (; iter != iterEnd; ++iter)
	{
		(*iter)->Render();
	}
}

bool CViewport::SortWindow(CSharedPtr<CWidgetWindow> Src, CSharedPtr<CWidgetWindow> Dest)
{
	return Src->GetZOrder() > Dest->GetZOrder();
}