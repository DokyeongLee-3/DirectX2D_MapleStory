
#include "EditorManager.h"
#include "Engine.h"
#include "Device.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/DefaultScene.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "IMGUITextInput.h"
#include "Window/SpriteWindow.h"
#include "Window/DetailWindow.h"
#include "Window/EditorMenu.h"
#include "Window/ObjectHierarchy.h"
#include "Object/DragObject.h"
#include "Object/Pivot.h"
#include "Render/RenderManager.h"
#include "Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "IMGUIImage.h"
#include "Object/Player2D.h"
#include "Engine.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_DragObj(nullptr),
	m_DragPivot(nullptr)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode Mode)
{
	m_EditMode = Mode;

	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
			m_DragObj->Destroy();

		m_DragObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CDragObject>("DragObject");
		m_DragObj->SetWorldScale(0.f, 0.f, 1.f);

		if (m_DragPivot)
			m_DragPivot->Destroy();

		m_DragPivot = CSceneManager::GetInst()->GetScene()->CreateGameObject<CPivot>("DragPivot");
		m_DragPivot->SetWorldScale(2.f, 2.f, 1.f);
		m_DragPivot->SetWorldPos(500.f, 500.f, 0.f);
		m_DragPivot->SetPivot(0.5f, 0.5f, 0.f);
	}
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	CEngine::GetInst()->SetPlay(false);

	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}


	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CEditorManager>(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	/*
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');
	CInput::GetInst()->CreateKey("Attack", VK_SPACE);
	CInput::GetInst()->CreateKey("Attack1", VK_LBUTTON);*/

	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("SpriteWindow");
	m_DetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("DetailWindow");
	m_EditorMenu = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");

	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Push, this, &CEditorManager::MouseLButtonPush);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Up, this, &CEditorManager::MouseLButtonUp);


	CInput::GetInst()->CreateKey("Up", VK_UP);
	CInput::GetInst()->CreateKey("Down", VK_DOWN);
	CInput::GetInst()->CreateKey("Left", VK_LEFT);
	CInput::GetInst()->CreateKey("Right", VK_RIGHT);

	CInput::GetInst()->SetKeyCallback("Up", KeyState_Down, this, &CEditorManager::KeyboardUp);
	CInput::GetInst()->SetKeyCallback("Down", KeyState_Down, this, &CEditorManager::KeyboardDown);
	CInput::GetInst()->SetKeyCallback("Left", KeyState_Down, this, &CEditorManager::KeyboardLeft);
	CInput::GetInst()->SetKeyCallback("Right", KeyState_Down, this, &CEditorManager::KeyboardRight);

	CInput::GetInst()->CreateKey("EditObjMoveDown", 'S');
	CInput::GetInst()->SetKeyCallback("EditObjMoveDown", KeyState_Push, this, &CEditorManager::EditObjDown);


	CInput::GetInst()->CreateKey("MoveUp", 'W');
	CInput::GetInst()->CreateKey("MoveDown", 'S');
	CInput::GetInst()->CreateKey("RotationZInv", 'A');
	CInput::GetInst()->CreateKey("RotationZ", 'D');

	return true;
}

void CEditorManager::CreateDefaultSceneMode()
{
	CSceneManager::GetInst()->CreateSceneMode<CDefaultScene>();
}

int CEditorManager::Run()
{
	return CEngine::GetInst()->Run();
}

void CEditorManager::MouseLButtonDown(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->SetStartPos(CInput::GetInst()->GetMousePos());

		Vector2 DragObjStartPos = m_DragObj->GetStartPos();

		Resolution RS = CDevice::GetInst()->GetResolution();

		if (DragObjStartPos.x < 0.f || DragObjStartPos.x > RS.Width ||
			DragObjStartPos.y < 0.f || DragObjStartPos.y > RS.Height)
		{
			return;
		}

		CIMGUITextInput* FrameStartPosX = m_SpriteWindow->GetFrameStartPosX();
		CIMGUITextInput* FrameStartPosY = m_SpriteWindow->GetFrameStartPosY();

		Vector2 PosInImage;
		CSpriteEditObject* SpriteObj = m_SpriteWindow->GetSpriteObject();
		CTexture* SpriteTex = SpriteObj->GetSpriteComponent()->GetMaterial()->GetTexture();

		unsigned int TexWidth = SpriteTex->GetWidth();
		unsigned int TexHeight = SpriteTex->GetHeight();

		PosInImage = Vector2(DragObjStartPos.x - SpriteObj->GetWorldPos().x, SpriteObj->GetWorldPos().y + TexHeight - DragObjStartPos.y);

		FrameStartPosX->SetInt((int)PosInImage.x);
		FrameStartPosY->SetInt((int)PosInImage.y);

	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	if (m_DragObj)
	{
		m_DragObj->SetEndPos(CInput::GetInst()->GetMousePos());

		Vector2 DragObjEndPos = m_DragObj->GetEndPos();

		Resolution RS = CDevice::GetInst()->GetResolution();

		if (DragObjEndPos.x < 0.f || DragObjEndPos.x > RS.Width ||
			DragObjEndPos.y < 0.f || DragObjEndPos.y > RS.Height)
		{
			return;
		}

		CIMGUITextInput* FrameEndPosX = m_SpriteWindow->GetFrameEndPosX();
		CIMGUITextInput* FrameEndPosY = m_SpriteWindow->GetFrameEndPosY();

		Vector2 PosInImage;
		CSpriteEditObject* SpriteObj = m_SpriteWindow->GetSpriteObject();
		CTexture* SpriteTex = SpriteObj->GetSpriteComponent()->GetMaterial()->GetTexture();

		unsigned int TexWidth = SpriteTex->GetWidth();
		unsigned int TexHeight = SpriteTex->GetHeight();

		PosInImage = Vector2(DragObjEndPos.x - SpriteObj->GetWorldPos().x, SpriteObj->GetWorldPos().y + TexHeight - DragObjEndPos.y);

		FrameEndPosX->SetInt((int)PosInImage.x);
		FrameEndPosY->SetInt((int)PosInImage.y);


		if (m_DragPivot)
		{
			Vector2 DragStartPos = m_DragObj->GetStartPos();
			Vector2 DragEndPos = m_DragObj->GetEndPos();

			m_DragPivot->SetWorldPos((DragStartPos.x + DragEndPos.x) / 2.f, (DragStartPos.y + DragEndPos.y) / 2.f, 0.f); 
		}
	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
}

void CEditorManager::KeyboardUp(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, 1.f, 0.f);

	CIMGUITextInput* StartYPos = m_SpriteWindow->GetFrameStartPosY();
	CIMGUITextInput* EndYPos = m_SpriteWindow->GetFrameEndPosY();

	StartYPos->SetInt(StartYPos->GetValueInt() - 1);
	EndYPos->SetInt(EndYPos->GetValueInt() - 1);

	m_SpriteWindow->AdjustFrameDataStartY();
	m_SpriteWindow->AdjustFrameDataEndY();
}

void CEditorManager::KeyboardDown(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, -1.f, 0.f);

	CIMGUITextInput* StartYPos = m_SpriteWindow->GetFrameStartPosY();
	CIMGUITextInput* EndYPos = m_SpriteWindow->GetFrameEndPosY();

	StartYPos->SetInt(StartYPos->GetValueInt() + 1);
	EndYPos->SetInt(EndYPos->GetValueInt() + 1);

	m_SpriteWindow->AdjustFrameDataStartY();
	m_SpriteWindow->AdjustFrameDataEndY();
}

void CEditorManager::KeyboardLeft(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(-1.f, 0.f, 0.f);

	CIMGUITextInput* StartXPos = m_SpriteWindow->GetFrameStartPosX();
	CIMGUITextInput* EndXPos = m_SpriteWindow->GetFrameEndPosX();

	StartXPos->SetInt(StartXPos->GetValueInt() - 1);
	EndXPos->SetInt(EndXPos->GetValueInt() - 1);

	m_SpriteWindow->AdjustFrameDataStartX();
	m_SpriteWindow->AdjustFrameDataEndX();
}

void CEditorManager::KeyboardRight(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(1.f, 0.f, 0.f);

	CIMGUITextInput* StartXPos = m_SpriteWindow->GetFrameStartPosX();
	CIMGUITextInput* EndXPos = m_SpriteWindow->GetFrameEndPosX();

	StartXPos->SetInt(StartXPos->GetValueInt() + 1);
	EndXPos->SetInt(EndXPos->GetValueInt() + 1);

	m_SpriteWindow->AdjustFrameDataStartX();
	m_SpriteWindow->AdjustFrameDataEndX();
}

void CEditorManager::EditObjDown(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(0.f, -50.f * DeltaTime, 0.f);
}

void CEditorManager::EditObjUp(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(0.f, 50.f * DeltaTime, 0.f);
}

void CEditorManager::EditObjLeft(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(-50.f * DeltaTime, 0.f, 0.f);
}

void CEditorManager::EditObjRight(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(50.f * DeltaTime, 0.f, 0.f);
}

void CEditorManager::CreateSceneMode(CScene* Scene, size_t Type)
{
	if (Type == typeid(CDefaultScene).hash_code())
	{
		Scene->LoadSceneMode<CDefaultScene>();
	}
}

CGameObject* CEditorManager::CreateObject(CScene* Scene, size_t Type)
{
	if (Type == typeid(CGameObject).hash_code())
	{
		CGameObject* Obj = Scene->LoadGameObject<CGameObject>();

		return Obj;
	}

	else if (Type == typeid(CDragObject).hash_code())
	{
		CDragObject* Obj = Scene->LoadGameObject<CDragObject>();

		return Obj;
	}

	else if (Type == typeid(CSpriteEditObject).hash_code())
	{
		CSpriteEditObject* Obj = Scene->LoadGameObject<CSpriteEditObject>();

		return Obj;
	}

	else if (Type == typeid(CPlayer2D).hash_code())
	{
		CPlayer2D* Obj = Scene->LoadGameObject<CPlayer2D>();

		return Obj;
	}

	return nullptr;
}

CComponent* CEditorManager::CreateComponent(CGameObject* Obj, size_t Type)
{
	if (Type == typeid(CSceneComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}

	else if (Type == typeid(CSpriteComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}

	else if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}

	return nullptr;
}

void CEditorManager::CreateAnimInstance(CSpriteComponent* Sprite, size_t Type)
{
	if (Type == typeid(CAnimationSequence2DInstance).hash_code())
	{
		Sprite->LoadAnimationInstance<CAnimationSequence2DInstance>();
	}
}
