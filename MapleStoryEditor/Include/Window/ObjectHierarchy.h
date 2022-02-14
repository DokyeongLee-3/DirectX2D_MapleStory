#pragma once

#include "IMGUIWindow.h"
#include "GameObject/GameObject.h"
#include "Component/SceneComponent.h"

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
	class CIMGUIButton* m_ObjectDeleteButton;
	class CIMGUITextInput* m_ZOrder;
	class CIMGUIButton* m_ZOrderChange;

	CSharedPtr<CGameObject>		m_SelectObject;
	CSharedPtr<CSceneComponent>	m_SelectComponent;

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

	CGameObject* GetSelectObject()	const
	{
		return m_SelectObject;
	}

	CSceneComponent* GetSelectComponent()	const
	{
		return m_SelectComponent;
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
	void DeleteObjectButtonCallback();
	void ZOrderChangeCallback();
};

