
#include "EditorManager.h"
#include "Engine.h"
#include "Device.h"
#include "resource.h"
#include "Scene/SceneManager.h"
#include "Scene/DefaultScene.h"
#include "Input.h"
#include "IMGUIManager.h"
#include "IMGUIText.h"
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
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "IMGUIImage.h"
#include "IMGUIListBox.h"
#include "ObjectSet.h"
#include "Engine.h"
#include "Collision/Collision.h"
#include "Scene/SceneCollision.h"

#include <sstream>

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
	CEngine::GetInst()->SetPlay(true);

	if (!CEngine::GetInst()->Init(hInst, TEXT("MapleStoryEditor"),
		1280, 720, IDI_ICON2))
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

	/*CInput::GetInst()->CreateKey("EditObjMoveDown", 'S');
	CInput::GetInst()->SetKeyCallback("EditObjMoveDown", KeyState_Push, this, &CEditorManager::EditObjDown);
	CInput::GetInst()->CreateKey("EditObjMoveUp", 'W');
	CInput::GetInst()->SetKeyCallback("EditObjMoveUp", KeyState_Push, this, &CEditorManager::EditObjUp);
	CInput::GetInst()->CreateKey("EditObjMoveRight", 'D');
	CInput::GetInst()->SetKeyCallback("EditObjMoveRight", KeyState_Push, this, &CEditorManager::EditObjRight);
	CInput::GetInst()->CreateKey("EditObjMoveLeft", 'A');
	CInput::GetInst()->SetKeyCallback("EditObjMoveLeft", KeyState_Push, this, &CEditorManager::EditObjLeft);*/

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
	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();
			m_DragObj->SetStartPos(MouseWorldPos);

			Vector2 DragObjStartPos = m_DragObj->GetStartPos();

			CIMGUITextInput* FrameStartPosX = m_SpriteWindow->GetFrameStartPosX();
			CIMGUITextInput* FrameStartPosY = m_SpriteWindow->GetFrameStartPosY();

			CSpriteEditObject* SpriteObj = m_SpriteWindow->GetSpriteObject();
			CTexture* SpriteTex = SpriteObj->GetSpriteComponent()->GetMaterial()->GetTexture();

			unsigned int TexHeight = SpriteTex->GetHeight();

			Vector2 PosInImage;
			PosInImage = Vector2(DragObjStartPos.x, TexHeight - MouseWorldPos.y);

			FrameStartPosX->SetFloat(PosInImage.x);
			FrameStartPosY->SetFloat(PosInImage.y);
		}
	}

	else if (m_EditMode == EditMode::Scene)
	{
		CColliderComponent* Comp = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();

		if (Comp)
		{
			Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

			CGameObject* Owner = Comp->GetGameObject();
			Owner->AddWorldPos(MouseMove.x, MouseMove.y, 0.f);

			m_DetailWindow->GetPosXInput()->SetValueFloat(Owner->GetWorldPos().x);
			m_DetailWindow->GetPosYInput()->SetValueFloat(Owner->GetWorldPos().y);
		}
	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();
			m_DragObj->SetEndPos(CInput::GetInst()->GetMouseWorld2DPos());

			Vector2 DragObjEndPos = m_DragObj->GetEndPos();

			CIMGUITextInput* FrameEndPosX = m_SpriteWindow->GetFrameEndPosX();
			CIMGUITextInput* FrameEndPosY = m_SpriteWindow->GetFrameEndPosY();

			CSpriteEditObject* SpriteObj = m_SpriteWindow->GetSpriteObject();
			CTexture* SpriteTex = SpriteObj->GetSpriteComponent()->GetMaterial()->GetTexture();

			unsigned int TexHeight = SpriteTex->GetHeight();

			Vector2 PosInImage;
			PosInImage = Vector2(DragObjEndPos.x, TexHeight - MouseWorldPos.y);

			FrameEndPosX->SetFloat(PosInImage.x);
			FrameEndPosY->SetFloat(PosInImage.y);


			if (m_DragPivot)
			{
				Vector2 DragStartPos = m_DragObj->GetStartPos();
				Vector2 DragEndPos = m_DragObj->GetEndPos();

				m_DragPivot->SetWorldPos((DragStartPos.x + DragEndPos.x) / 2.f, (DragStartPos.y + DragEndPos.y) / 2.f, 0.f);

				float FrameCenterX = (m_SpriteWindow->GetFrameStartPosX()->GetValueFloat() + m_SpriteWindow->GetFrameEndPosX()->GetValueFloat()) / 2.f;
				float FrameCenterY = (m_SpriteWindow->GetFrameStartPosY()->GetValueFloat() + m_SpriteWindow->GetFrameEndPosY()->GetValueFloat()) / 2.f;

				Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

				std::stringstream PosX;
				PosX << Center.x;
				std::stringstream PosY;
				PosY << Center.y;

				m_SpriteWindow->GetDragPivotXPosText()->SetText(PosX.str().c_str());
				m_SpriteWindow->GetDragPivotYPosText()->SetText(PosY.str().c_str());
			}
		}
	}

	if (m_EditMode == EditMode::Scene)
	{
		CColliderComponent* Comp = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();

		if (Comp)
		{
			Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

			CGameObject* Owner = Comp->GetGameObject();
			Owner->AddWorldPos(MouseMove.x, MouseMove.y, 0.f);

			m_DetailWindow->GetPosXInput()->SetValueFloat(Owner->GetWorldPos().x);
			m_DetailWindow->GetPosYInput()->SetValueFloat((Owner->GetWorldPos().y));
		}


	}
}

void CEditorManager::MouseLButtonUp(float DeltaTime)
{
}

void CEditorManager::MouseClickCallback(float DeltaTime)
{
}

void CEditorManager::EditObjDown(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	if (!Obj)
		return;

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(0.f, -200.f * DeltaTime, 0.f);
}

void CEditorManager::EditObjUp(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	if (!Obj)
		return;

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(0.f, 200.f * DeltaTime, 0.f);
}

void CEditorManager::EditObjLeft(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	if (!Obj)
		return;

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(-200.f * DeltaTime, 0.f, 0.f);
}

void CEditorManager::EditObjRight(float DeltaTime)
{
	CSpriteEditObject* Obj = m_SpriteWindow->GetSpriteObject();

	if (!Obj)
		return;

	Vector3 Pos = Obj->GetWorldPos();

	Obj->AddWorldPos(200.f * DeltaTime, 0.f, 0.f);
}

void CEditorManager::CreateSceneMode(CScene* Scene, size_t Type)
{
	if (Type == typeid(CDefaultScene).hash_code())
	{
		if(!(Scene->GetSceneMode()))
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

	else if (Type == typeid(CMonsterRadish).hash_code())
	{
		CMonsterRadish* Obj = Scene->LoadGameObject<CMonsterRadish>();

		return Obj;
	}

	else if (Type == typeid(CMonsterOnion).hash_code())
	{
		CMonsterOnion* Obj = Scene->LoadGameObject<CMonsterOnion>();

		return Obj;
	}

	else if (Type == typeid(CPlayerSkillSet).hash_code())
	{
		CPlayerSkillSet* Obj = Scene->LoadGameObject<CPlayerSkillSet>();

		return Obj;
	}

	else if (Type == typeid(CPortal).hash_code())
	{
		CPortal* Obj = Scene->LoadGameObject<CPortal>();

		return Obj;
	}

	else if (Type == typeid(CLampLight).hash_code())
	{
		CLampLight* Obj = Scene->LoadGameObject<CLampLight>();

		return Obj;
	}

	else if (Type == typeid(CBlinkTree).hash_code())
	{
		CBlinkTree* Obj = Scene->LoadGameObject<CBlinkTree>();

		return Obj;
	}

	else if (Type == typeid(CButterfly).hash_code())
	{
		CButterfly* Obj = Scene->LoadGameObject<CButterfly>();

		return Obj;
	}

	else if (Type == typeid(CDoubleHelixBlinkTree).hash_code())
	{
		CDoubleHelixBlinkTree* Obj = Scene->LoadGameObject<CDoubleHelixBlinkTree>();

		return Obj;
	}

	else if (Type == typeid(CSingleHelixBlinkTree).hash_code())
	{
		CSingleHelixBlinkTree* Obj = Scene->LoadGameObject<CSingleHelixBlinkTree>();

		return Obj;
	}


	else if (Type == typeid(CLowerClassBook).hash_code())
	{
		CLowerClassBook* Obj = Scene->LoadGameObject<CLowerClassBook>();

		return Obj;
	}

	else if (Type == typeid(CLibrary2ndLampLight).hash_code())
	{
		CLibrary2ndLampLight* Obj = Scene->LoadGameObject<CLibrary2ndLampLight>();

		return Obj;
	}

	else if (Type == typeid(CLibrary2ndButterfly).hash_code())
	{
		CLibrary2ndButterfly* Obj = Scene->LoadGameObject<CLibrary2ndButterfly>();

		return Obj;
	}

	else if (Type == typeid(CCharacterSelectBackLight).hash_code())
	{
		CCharacterSelectBackLight* Obj = Scene->LoadGameObject<CCharacterSelectBackLight>();

		return Obj;
	}

	else if (Type == typeid(CStaticMapObj).hash_code())
	{
		CStaticMapObj* Obj = Scene->LoadGameObject<CStaticMapObj>();

		return Obj;
	}

	else if (Type == typeid(CStage).hash_code())
	{
		CStage* Obj = Scene->LoadGameObject<CStage>();

		CDefaultScene* DefaultScene = (CDefaultScene*)CSceneManager::GetInst()->GetScene()->GetSceneMode();
		DefaultScene->SetStageObject(Obj);

		return Obj;
	}

	return nullptr;
}

CComponent* CEditorManager::CreateComponent(CGameObject* Obj, size_t Type)
{
	if (Type == typeid(CSceneComponent).hash_code())
	{
		CSceneComponent* Component = Obj->LoadComponent<CSceneComponent>();

		return Component;
	}

	else if (Type == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Component = Obj->LoadComponent<CSpriteComponent>();

		return Component;
	}

	else if (Type == typeid(CStaticMeshComponent).hash_code())
	{
		CStaticMeshComponent* Component = Obj->LoadComponent<CStaticMeshComponent>();

		return Component;
	}

	else if (Type == typeid(CColliderBox2D).hash_code())
	{
		CColliderBox2D* Component = Obj->LoadComponent<CColliderBox2D>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

		return Component;
	}

	else if (Type == typeid(CColliderCircle).hash_code())
	{
		CColliderCircle* Component = Obj->LoadComponent<CColliderCircle>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

		return Component;
	}

	else if (Type == typeid(CColliderPixel).hash_code())
	{
		CColliderPixel* Component = Obj->LoadComponent<CColliderPixel>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

		return Component;
	}

	else if (Type == typeid(CDragCollider).hash_code())
	{
		CDragCollider* Component = Obj->LoadComponent<CDragCollider>();

		CSceneManager::GetInst()->GetScene()->GetCollision()->AddCollider(Component);

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

void CEditorManager::AddObjectHierarchy()
{
	int ObjCount = (int)CSceneManager::GetInst()->GetScene()->GetObjectCount();

	for (int i = 0; i < ObjCount; ++i)
	{
		CGameObject* Obj = CSceneManager::GetInst()->GetScene()->GetGameObject(i);

		m_ObjectHierarchy->AddObjectList(Obj->GetName().c_str());

		AddComponentHierarchy(Obj);
	}
}

void CEditorManager::AddComponentHierarchy(CGameObject* Obj)
{
	CSceneComponent* Com = Obj->GetRootComponent();

	std::vector<FindComponentName> vecSceneComp;
	Com->GetAllSceneComponentsName(vecSceneComp);

	int Count = (int)vecSceneComp.size();

	for (int i = 0; i < Count; ++i)
	{
		m_ObjectHierarchy->AddComponentList(vecSceneComp[i].Name.c_str());
	}
}

void CEditorManager::AddComponentHierarchy(const char* ComponentName)
{
	m_ObjectHierarchy->AddComponentList(ComponentName);
}
