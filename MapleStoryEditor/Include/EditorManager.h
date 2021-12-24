#pragma once

#include "GameInfo.h"

enum class EditMode
{
	Scene,
	Sprite
};

class CEditorManager
{
private:
	EditMode	m_EditMode;
	// Scene의 m_ObjList에서 Ref Count = 1인채로
	// 계속 존재하므로 알아서 메모리 해제될 것임
	class CDragObject* m_DragObj;
	class CPivot* m_DragPivot;
	class CSpriteWindow* m_SpriteWindow;
	class CDetailWindow* m_DetailWindow;
	class CEditorMenu* m_EditorMenu;
	class CConfigurationWindow* m_ConfigurationWindow;
	class CObjectHierarchy* m_ObjectHierarchy;

public:
	EditMode GetEditMode()
	{
		return m_EditMode;
	}

	class CDragObject* GetDragObj()	const
	{
		return m_DragObj;
	}

	class CPivot* GetDragPivot()	const
	{
		return m_DragPivot;
	}

	void SetEditMode(EditMode Mode);

public:
	bool Init(HINSTANCE hInst);
	void CreateDefaultSceneMode();
	int Run();

public:
	void MouseLButtonDown(float DeltaTime);
	void MouseLButtonPush(float DeltaTime);
	void MouseLButtonUp(float DeltaTime);

	void KeyboardUp(float DeltaTime);
	void KeyboardDown(float DeltaTime);
	void KeyboardLeft(float DeltaTime);
	void KeyboardRight(float DeltaTime);

	void EditObjDown(float DeltaTime);
	void EditObjUp(float DeltaTime);
	void EditObjLeft(float DeltaTime);
	void EditObjRight(float DeltaTime);

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);


	DECLARE_SINGLE(CEditorManager)
};

