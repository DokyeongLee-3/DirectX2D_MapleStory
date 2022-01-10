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
	class CIMGUIComboBox* m_LayerCombo;
	class CIMGUIButton* m_LayerChangeButton;
	class CIMGUILabel* m_ObjectLayerLabel;
	class CIMGUIText* m_ObjectLayer;

public:
	class CIMGUIComboBox* GetLayerCombo()	const
	{
		return m_LayerCombo;
	}

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
	void AddComponentList(const char* Name);

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void SelectObject(int Index, const char* Item);
	void SelectComponent(int Index, const char* Item);
	void LayerChangeCallback();
};

