#pragma once

#include "GameInfo.h"

enum class EditMode
{
	Map,
	Sprite,
	Scene
};

class CEditorManager
{
private:
	EditMode	m_EditMode;
	// Scene�� m_ObjList���� Ref Count = 1��ä��
	// ��� �����ϹǷ� �˾Ƽ� �޸� ������ ����
	class CDragObject* m_DragObj;
	class CPivot* m_DragPivot;
	class CSpriteWindow* m_SpriteWindow;
	class CDetailWindow* m_DetailWindow;
	class CEditorMenu* m_EditorMenu;
	class CConfigurationWindow* m_ConfigurationWindow;
	class CObjectHierarchy* m_ObjectHierarchy;
	class CTileMapWindow* m_TileMapWindow;
	// ���콺�� ���������ӿ� �巡���ϴ� Collider
	class CColliderComponent* m_PrevCollision;


public:
	class CEditorMenu* GetEditorMenu()	const
	{
		return m_EditorMenu;
	}

	class CObjectHierarchy* GetObjectHierarchy()	const
	{
		return m_ObjectHierarchy;
	}

	class CSpriteWindow* GetSpriteWindow()	const
	{
		return m_SpriteWindow;
	}

	class CDetailWindow* GetDetailWindow()	const
	{
		return m_DetailWindow;
	}

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

	void MouseClickCallback(float DeltaTime);

	void EditObjDown(float DeltaTime);
	void EditObjUp(float DeltaTime);
	void EditObjLeft(float DeltaTime);
	void EditObjRight(float DeltaTime);

public:
	void CreateSceneMode(class CScene* Scene, size_t Type);
	class CGameObject* CreateObject(class CScene* Scene, size_t Type);
	class CComponent* CreateComponent(class CGameObject* Obj, size_t Type);
	void CreateAnimInstance(class CSpriteComponent* Sprite, size_t Type);

public:
	void AddObjectHierarchy();
	void AddComponentHierarchy(class CGameObject* Obj);
	void AddComponentHierarchy(const char* ComponentName);

	DECLARE_SINGLE(CEditorManager)
};

