
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
#include "Window/TileMapWindow.h"
#include "Window/ParticleSystemWindow.h"
#include "Object/DragObject.h"
#include "Object/Pivot.h"
#include "Render/RenderManager.h"
#include "Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/ParticleComponent.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/OnionMonsterAnimation.h"
#include "Animation/LowerClassBookAnimation.h"
#include "Animation/RadishMonsterAnimation.h"
#include "Animation/ZakumBodyAnimation.h"
#include "Animation/ZakumHandAnimation.h"
#include "Animation/PukoAnimation.h"
#include "Animation/PunkoAnimation.h"
#include "Animation/CuzcoAnimation.h"
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
	m_DragPivot(nullptr),
	m_PrevCollision(nullptr)
{
}

CEditorManager::~CEditorManager()
{
	CEngine::DestroyInst();
}

void CEditorManager::SetEditMode(EditMode Mode)
{
	m_EditMode = Mode;

	switch (m_EditMode)
	{
	case EditMode::Scene:
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		if (m_DragPivot)
		{
			m_DragPivot->Destroy();
			m_DragPivot = nullptr;
		}

		CSpriteWindow* SpriteWindow = m_SpriteWindow;
		if (SpriteWindow)
		{
			SpriteWindow->SetSpriteFrameTexture("DefaultUI", TEXT("NoImage.png"));
			SpriteWindow->SetSpriteFrameImageStart(Vector2(0.f, 0.f));
			SpriteWindow->SetSpriteFrameImageEnd(Vector2(100.f, 100.f));
		}
	}
		break;
	case EditMode::Sprite:
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
		break;
	case EditMode::TileMap:
	{
		if (m_DragObj)
		{
			m_DragObj->Destroy();
			m_DragObj = nullptr;
		}

		if (m_DragPivot)
		{
			m_DragPivot->Destroy();
			m_DragPivot = nullptr;
		}

		CSpriteWindow* SpriteWindow = m_SpriteWindow;
		if (SpriteWindow)
		{
			SpriteWindow->SetSpriteFrameTexture("DefaultUI", TEXT("NoImage.png"));
			SpriteWindow->SetSpriteFrameImageStart(Vector2(0.f, 0.f));
			SpriteWindow->SetSpriteFrameImageEnd(Vector2(100.f, 100.f));
		}
	}
		break;
	}

}

bool CEditorManager::Init(HINSTANCE hInst)
{
	//CEngine::GetInst()->SetPlay(true);

	if (!CEngine::GetInst()->Init(hInst, TEXT("MapleStoryEditor"),
		1280, 720, IDI_ICON2))
	{
		CEngine::DestroyInst();
		return false;
	}

	CEngine::GetInst()->EnableClientMode(false);

	CSceneManager::GetInst()->SetCreateSceneModeFunction<CEditorManager>(this, &CEditorManager::CreateSceneMode);
	CSceneManager::GetInst()->SetCreateObjectFunction<CEditorManager>(this, &CEditorManager::CreateObject);
	CSceneManager::GetInst()->SetCreateComponentFunction<CEditorManager>(this, &CEditorManager::CreateComponent);
	CSceneManager::GetInst()->SetCreateAnimInstanceFunction<CEditorManager>(this, &CEditorManager::CreateAnimInstance);

	// IMGUI로 에디터에서 사용할 윈도우를 만들어준다.
	m_SpriteWindow = CIMGUIManager::GetInst()->AddWindow<CSpriteWindow>("SpriteWindow");
	m_DetailWindow = CIMGUIManager::GetInst()->AddWindow<CDetailWindow>("DetailWindow");	
	m_EditorMenu = CIMGUIManager::GetInst()->AddWindow<CEditorMenu>("EditorMenu");
	m_ObjectHierarchy = CIMGUIManager::GetInst()->AddWindow<CObjectHierarchy>("ObjectHierarchy");
	m_TileMapWindow = CIMGUIManager::GetInst()->AddWindow<CTileMapWindow>("TileMapWindow");
	m_ParticleWindow = CIMGUIManager::GetInst()->AddWindow<CParticleSystemWindow>("ParticleSystemWindow");

	CRenderManager::GetInst()->CreateLayer("DragLayer", INT_MAX);

	CInput::GetInst()->CreateKey("MouseLButton", VK_LBUTTON);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Down, this, &CEditorManager::MouseLButtonDown);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Push, this, &CEditorManager::MouseLButtonPush);
	CInput::GetInst()->SetKeyCallback("MouseLButton", KeyState_Up, this, &CEditorManager::MouseLButtonUp);

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
		m_PrevCollision = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();
		//CColliderComponent* Comp = CSceneManager::GetInst()->GetScene()->GetCollision()->GetMouseCollision();

		if (m_PrevCollision)
		{
			Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

			CGameObject* Owner = m_PrevCollision->GetGameObject();
			Owner->AddWorldPos(MouseMove.x, MouseMove.y, 0.f);

			m_DetailWindow->GetPosXInput()->SetValueFloat(Owner->GetWorldPos().x);
			m_DetailWindow->GetPosYInput()->SetValueFloat(Owner->GetWorldPos().y);
		}
	}
}

void CEditorManager::MouseLButtonPush(float DeltaTime)
{
	m_MousePush = true;

	if (m_EditMode == EditMode::Sprite)
	{
		if (m_DragObj)
		{
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();
			m_DragObj->SetEndPos(MouseWorldPos);

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

		if (m_PrevCollision)
		{
			if (Comp != m_PrevCollision)
			{
				Comp = m_PrevCollision;
			}
		}

		else
			m_PrevCollision = Comp;

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
	m_MousePush = false;
	m_PrevCollision = nullptr;
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

	else if (Type == typeid(CRadishMonster).hash_code())
	{
		CRadishMonster* Obj = Scene->LoadGameObject<CRadishMonster>();

		return Obj;
	}

	else if (Type == typeid(COnionMonster).hash_code())
	{
		COnionMonster* Obj = Scene->LoadGameObject<COnionMonster>();

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

	else if (Type == typeid(CLobbyBigLamp).hash_code())
	{
		CLobbyBigLamp* Obj = Scene->LoadGameObject<CLobbyBigLamp>();

		return Obj;
	}

	else if (Type == typeid(CLobbySmallLamp).hash_code())
	{
		CLobbySmallLamp* Obj = Scene->LoadGameObject<CLobbySmallLamp>();

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

	else if (Type == typeid(CTileObject).hash_code())
	{
		CTileObject* Obj = Scene->LoadGameObject<CTileObject>();

		return Obj;
	}

	else if (Type == typeid(CStage).hash_code())
	{
		CStage* Obj = Scene->LoadGameObject<CStage>();

		CDefaultScene* DefaultScene = (CDefaultScene*)CSceneManager::GetInst()->GetScene()->GetSceneMode();
		DefaultScene->SetStageObject(Obj);

		return Obj;
	}

	else if (Type == typeid(CVerticalLampLight).hash_code())
	{
		CVerticalLampLight* Obj = Scene->LoadGameObject<CVerticalLampLight>();

		return Obj;
	}

	else if (Type == typeid(CLopeTileObject).hash_code())
	{
		CLopeTileObject* Obj = Scene->LoadGameObject<CLopeTileObject>();

		return Obj;
	}

	else if (Type == typeid(CNPCAdobis).hash_code())
	{
		CNPCAdobis* Obj = Scene->LoadGameObject<CNPCAdobis>();

		return Obj;
	}

	else if (Type == typeid(CZakumBody).hash_code())
	{
		CZakumBody* Obj = Scene->LoadGameObject<CZakumBody>();

		return Obj;
	}

	else if (Type == typeid(CNPCAmon).hash_code())
	{
		CNPCAmon* Obj = Scene->LoadGameObject<CNPCAmon>();

		return Obj;
	}

	else if (Type == typeid(CPuko).hash_code())
	{
		CPuko* Obj = Scene->LoadGameObject<CPuko>();

		return Obj;
	}

	else if (Type == typeid(CPunko).hash_code())
	{
		CPunko* Obj = Scene->LoadGameObject<CPunko>();

		return Obj;
	}

	else if (Type == typeid(CCuzco).hash_code())
	{
		CCuzco* Obj = Scene->LoadGameObject<CCuzco>();

		return Obj;
	}

	else if (Type == typeid(CParticleObject).hash_code())
	{
		CParticleObject* Obj = Scene->LoadGameObject<CParticleObject>();

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

		Component->SetColliderColor(Vector4(0.f, 0.f, 1.f, 1.f));

		return Component;
	}

	else if (Type == typeid(CTileMapComponent).hash_code())
	{
		CTileMapComponent* Component = Obj->LoadComponent<CTileMapComponent>();

		Component->EnableEditMode(true);

		return Component;
	}

	else if (Type == typeid(CParticleComponent).hash_code())
	{
		CParticleComponent* Component = Obj->LoadComponent<CParticleComponent>();

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

	else if (Type == typeid(COnionMonsterAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<COnionMonsterAnimation>();
	}

	else if (Type == typeid(CRadishMonsterAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<COnionMonsterAnimation>();
	}

	else if (Type == typeid(CLowerClassBookAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CLowerClassBookAnimation>();
	}

	else if (Type == typeid(CZakumBodyAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CZakumBodyAnimation>();
	}

	else if (Type == typeid(CZakumHandAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CZakumHandAnimation>();
	}

	else if (Type == typeid(CPukoAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CPukoAnimation>();
	}

	else if (Type == typeid(CPunkoAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CPunkoAnimation>();
	}

	else if (Type == typeid(CCuzcoAnimation).hash_code())
	{
		Sprite->LoadAnimationInstance<CCuzcoAnimation>();
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
