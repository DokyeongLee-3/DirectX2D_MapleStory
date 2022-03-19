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
	class CIMGUIButton* m_ComponentDeleteButton;

	class CIMGUIComboBox* m_ProfileCombo;
	class CIMGUIButton* m_ProfileChangeButton;
	class CIMGUIText* m_Profile;

	CSharedPtr<CGameObject>		m_SelectObject;
	CSharedPtr<CSceneComponent>	m_SelectComponent;

	class CIMGUIText* m_ParentComponent;

public:
	void SetParentComponent(const char* Parent);

	CIMGUIComboBox* GetLayerCombo()	const
	{
		return m_LayerCombo;
	}

	CIMGUIListBox* GetObjectList()	const
	{
		return m_ObjectListWidget;
	}

	CIMGUIListBox* GetComponentList()	const
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

	class CIMGUIText* GetProfileText()	const
	{
		return m_Profile;
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
	void DeleteComponentButtonCallback();
	void ZOrderChangeCallback();
	void ProfileChangeCallback();

public:
	void ClearHierarchyWindowInfo();
};

