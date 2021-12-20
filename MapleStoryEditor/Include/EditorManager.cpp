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
#include "Window/ConfigurationWindow.h"
#include "Window/ObjectHierarchy.h"
#include "Object/DragObject.h"
#include "Render/RenderManager.h"
#include "Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "IMGUIImage.h"

DEFINITION_SINGLE(CEditorManager)

CEditorManager::CEditorManager() :
	m_EditMode(EditMode::Scene),
	m_SpriteWindow(nullptr)
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
	}
}

bool CEditorManager::Init(HINSTANCE hInst)
{
	if (!CEngine::GetInst()->Init(hInst, TEXT("GameEngine"),
		1280, 720, IDI_ICON1))
	{
		CEngine::DestroyInst();
		return false;
	}

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);

	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("SpriteWindow");
	m_DetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("DetailWindow");
	m_EditorMenu = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");
	m_ConfigurationWindow = CIMGUIManager::GetInst()->AddWindow<CConfigurationWindow>("Configuration Window");

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

	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
}

void CEditorManager::KeyboardUp(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, 1.f, 0.f);
}

void CEditorManager::KeyboardDown(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(0.f, -1.f, 0.f);
}

void CEditorManager::KeyboardLeft(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(-1.f, 0.f, 0.f);
}

void CEditorManager::KeyboardRight(float DeltaTime)
{
	if (m_DragObj)
		m_DragObj->AddWorldPos(1.f, 0.f, 0.f);
}

void CEditorManager::CreateSceneMode(CScene* Scene, size_t Type)
{
}

void CEditorManager::CreateObject(CScene* Scene, size_t Type)
{
}
