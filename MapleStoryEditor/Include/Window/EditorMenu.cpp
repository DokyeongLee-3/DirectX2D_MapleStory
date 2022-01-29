
#include "Engine.h"
#include "PathManager.h"
#include "EditorMenu.h"
#include "../EditorManager.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUIManager.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "../Scene/DefaultScene.h"
#include "Scene/SceneResource.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "ObjectHierarchy.h"
#include "SpriteWindow.h"
#include "Component/ColliderCircle.h"
#include "../ObjectSet.h"

CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();

	m_ObjectCombo = AddWidget<CIMGUIComboBox>("ObjectCombo", 100.f, 30.f);

	m_ObjectCombo->SetHideName(true);
	m_ObjectCombo->AddItem("GameObject");
	m_ObjectCombo->AddItem("Player");
	m_ObjectCombo->AddItem("MonsterRadish");
	m_ObjectCombo->AddItem("MonsterOnion");
	m_ObjectCombo->AddItem("MonsterLowerClassBook");
	m_ObjectCombo->AddItem("PlayerSkillSet");
	m_ObjectCombo->AddItem("Portal");
	m_ObjectCombo->AddItem("LampLight");
	m_ObjectCombo->AddItem("BlinkTree");
	m_ObjectCombo->AddItem("Butterfly");
	m_ObjectCombo->AddItem("DoubleHelixBlinkTree");
	m_ObjectCombo->AddItem("SingleHelixBlinkTree");
	m_ObjectCombo->AddItem("Library2ndLampLight");
	m_ObjectCombo->AddItem("Library2ndButterfly");
	m_ObjectCombo->AddItem("CharacterSelectBackLight");
	m_ObjectCombo->AddItem("StaticMapObj");
	m_ObjectCombo->AddItem("Stage");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 100.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectCreateButton = AddWidget<CIMGUIButton>("CreateObject", 100.f, 30.f);

	m_ObjectCreateButton->SetClickCallback(this, &CEditorMenu::ObjectCreateButton);


	// Component
	m_ComponentCombo = AddWidget<CIMGUIComboBox>("ComponentCombo", 100.f, 30.f);

	m_ComponentCombo->SetHideName(true);
	m_ComponentCombo->AddItem("SpriteComponent");
	m_ComponentCombo->AddItem("StaticMeshComponent");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("ComponentName", 100.f, 30.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentCreateButton = AddWidget<CIMGUIButton>("CreateComponent", 100.f, 30.f);

	m_ComponentCreateButton->SetClickCallback(this, &CEditorMenu::ComponentCreateButton);


	CIMGUIButton* SaveSceneButton = AddWidget<CIMGUIButton>("SaveSceneButton", 120.f, 30.f);

	SaveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	Line = AddWidget<CIMGUISameLine>("Line");

	CIMGUIButton* LoadSceneButton = AddWidget<CIMGUIButton>("LoadSceneButton", 120.f, 30.f);

	LoadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);



	CIMGUIButton* SelectMapObjTexture = AddWidget<CIMGUIButton>("Select Object Texture", 150.f, 30.f);

	SelectMapObjTexture->SetClickCallback(this, &CEditorMenu::SelectObjTexture);


	CIMGUIButton* PlayButton = AddWidget<CIMGUIButton>("Play/Stop", 100.f, 50.f);

	PlayButton->SetClickCallback(this, &CEditorMenu::GamePlay);


	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CEditorMenu::ObjectCreateButton()
{
	int	SelectIndex = m_ObjectCombo->GetSelectIndex();
	if (SelectIndex == -1)
		return;

	switch ((CreateObject_Type)SelectIndex)
	{
	case CreateObject_Type::GameObject:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CGameObject>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Player:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayer2D>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::MonsterRadish:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CMonsterRadish>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::MonsterOnion:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CMonsterOnion>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::PlayerSkillSet:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPlayerSkillSet>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Portal:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPortal>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::LampLight:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLampLight>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::BlinkTree:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CBlinkTree>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Butterfly:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CButterfly>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::DoubleHelixBlinkTree:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CDoubleHelixBlinkTree>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::SingleHelixBlinkTree:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CSingleHelixBlinkTree>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::LowerClassBook:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLowerClassBook>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Library2ndLampLight:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLibrary2ndLampLight>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Library2ndButterfly:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLibrary2ndButterfly>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::CharacterSelectBackLight:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CCharacterSelectBackLight>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::StaticMapObj:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CStaticMapObj>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Stage:
	{
		CDefaultScene* DefaultScene = ((CDefaultScene*)CSceneManager::GetInst()->GetScene()->GetSceneMode());
		if (!DefaultScene->GetStageObject())
		{
			CStage* Stage = CSceneManager::GetInst()->GetScene()->CreateGameObject<CStage>(m_ObjectNameInput->GetTextMultibyte());
			((CDefaultScene*)CSceneManager::GetInst()->GetScene()->GetSceneMode())->SetStageObject(Stage);
		}
	}
		break;
	}

	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (Hierarchy)
	{
		Hierarchy->AddObjectList(m_ObjectNameInput->GetTextMultibyte());
	}
}

void CEditorMenu::ComponentCreateButton()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	CGameObject* Obj = nullptr;

	if (Hierarchy)
	{
		CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

		if (ObjList->GetSelectIndex() == -1)
			return;

		Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());
	}

	if (!Obj)
		return;

	// 오브젝트 생성.
	int	SelectIndex = m_ComponentCombo->GetSelectIndex();
	if (SelectIndex == -1)
		return;

	CSceneComponent* Com = nullptr;

	switch ((SceneComponent_Type)SelectIndex)
	{
	case SceneComponent_Type::Sprite:
		Com = Obj->CreateComponent<CSpriteComponent>(m_ComponentNameInput->GetTextMultibyte());
		if(!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		break;
	case SceneComponent_Type::StaticMesh:
		Com = Obj->CreateComponent<CStaticMeshComponent>(m_ComponentNameInput->GetTextMultibyte());
		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		break;
	case SceneComponent_Type::ColliderBox2D:
		Com = Obj->CreateComponent<CColliderBox2D>(m_ComponentNameInput->GetTextMultibyte());
		Obj->GetRootComponent()->AddChild(Com);
		break;
	case SceneComponent_Type::ColliderCircle:
		Com = Obj->CreateComponent<CColliderCircle>(m_ComponentNameInput->GetTextMultibyte());
		Obj->GetRootComponent()->AddChild(Com);
		break;
	case SceneComponent_Type::DragCollider:
		Com = Obj->CreateComponent<CDragCollider>(m_ComponentNameInput->GetTextMultibyte());
		Obj->GetRootComponent()->AddChild(Com);
		break;
	}

	if (Hierarchy)
	{
		CIMGUIListBox* ComponentList = Hierarchy->GetComponentList();

		ComponentList->AddItem(m_ComponentNameInput->GetTextMultibyte());
	}
}

void CEditorMenu::SaveScene()
{
	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Scene File\0*.scn");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char    ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);

		CSceneManager::GetInst()->GetScene()->SaveFullPath(ConvertFullPath);
	}
}

void CEditorMenu::LoadScene()
{
	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Scene File\0*.scn");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char    ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);

		CSceneManager::GetInst()->GetScene()->LoadFullPath(ConvertFullPath);
	}
}

void CEditorMenu::GamePlay()
{
	if (!CEngine::GetInst()->IsPlay())
		CEngine::GetInst()->SetPlay(true);

	else
		CEngine::GetInst()->SetPlay(false);
}

void CEditorMenu::SelectObjTexture()
{
	if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
	{
		int SelectIdx = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectIndex();

		if (SelectIdx == -1)
			return;

		const std::string& SelectObjectName = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectItem();

		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(SelectObjectName);

		if (!SelectObject)
			return;

		if (SelectObject->GetTypeID() != typeid(CStage).hash_code() && SelectObject->GetTypeID() != typeid(CStaticMapObj).hash_code())
			return;

		if (SelectObject->GetRootComponent()->GetTypeID() != typeid(CSpriteComponent).hash_code())
			return;

		//////////
		TCHAR   FilePath[MAX_PATH] = {};

		OPENFILENAME    OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
		OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
		OpenFile.lpstrFile = FilePath;
		OpenFile.nMaxFile = MAX_PATH;
		OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

		if (GetOpenFileName(&OpenFile) != 0)
		{
			TCHAR   FileName[MAX_PATH] = {};
			TCHAR   FileExt[MAX_PATH] = {};

			_wsplitpath_s(FilePath, 0, 0, 0, 0, FileName, MAX_PATH, FileExt, MAX_PATH);

			char    ConvertFileName[MAX_PATH] = {};
			char    ConvertFileExt[MAX_PATH] = {};

			int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, 0, 0);

			Length = WideCharToMultiByte(CP_ACP, 0, FileExt, -1, 0, 0, 0, 0);
			WideCharToMultiByte(CP_ACP, 0, FileExt, -1, ConvertFileExt, Length, 0, 0);

			CSpriteComponent* SpriteComp = ((CStaticMapObj*)SelectObject)->GetSpriteComponent();

			SpriteComp->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);

			SpriteComp->SetWorldScale((float)SpriteComp->GetMaterial()->GetTextureWidth(), (float)SpriteComp->GetMaterial()->GetTextureHeight(), 1.f);
		}
	}
}


