#pragma once

#include "IMGUIWindow.h"

class CObjectHierarchy :
	public CIMGUIWindow
{
public:
	CObjectHierarchy();
	virtual ~CObjectHierarchy();

private:
	class CIMGUIListBox* m_ObjectListWidget;
	class CIMGUIListBox* m_ComponentListWidget;

public:
	class CIMGUIListBox* GetObjectList()	const
	{
		return m_ObjectListWidget;
	}

	class CIMGUIListBox* GetComponentList()	const
	{
		return m_ComponentListWidget;
	}

public:
	void AddObjectList(const char* Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void SelectObject(int Index, const char* Item);
	void SelectComponent(int Index, const char* Item);
};

