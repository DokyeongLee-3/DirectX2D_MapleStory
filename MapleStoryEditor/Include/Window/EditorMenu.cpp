
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
#include "Scene/SceneResource.h"
#include "../Scene/DefaultScene.h"
#include "Component/SpriteComponent.h"
#include "Component/StaticMeshComponent.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/CameraComponent.h"
#include "Component/WidgetComponent.h"
#include "Component/ParticleComponent.h"
#include "Component/TileMapComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "ObjectHierarchy.h"
#include "SpriteWindow.h"
#include "Component/ColliderCircle.h"
#include "../ObjectSet.h"
#include "DetailWindow.h"


CEditorMenu::CEditorMenu()
{
}

CEditorMenu::~CEditorMenu()
{
}

bool CEditorMenu::Init()
{
	CIMGUIWindow::Init();

	m_ObjectCombo = AddWidget<CIMGUIComboBox>("Select Object", 120.f, 26.f);

	m_ObjectCombo->SetHideName(true);
	m_ObjectCombo->AddItem("GameObject");
	m_ObjectCombo->AddItem("Player");
	m_ObjectCombo->AddItem("MonsterRadish");
	m_ObjectCombo->AddItem("MonsterOnion");
	m_ObjectCombo->AddItem("MonsterLowerClassBook");
	m_ObjectCombo->AddItem("PlayerSkillSet");
	m_ObjectCombo->AddItem("Portal");
	m_ObjectCombo->AddItem("LampLight");
	m_ObjectCombo->AddItem("LobbyBigLamp");
	m_ObjectCombo->AddItem("LobbySmallLamp");
	m_ObjectCombo->AddItem("BlinkTree");
	m_ObjectCombo->AddItem("Butterfly");
	m_ObjectCombo->AddItem("DoubleHelixBlinkTree");
	m_ObjectCombo->AddItem("SingleHelixBlinkTree");
	m_ObjectCombo->AddItem("CharacterSelectBackLight");
	m_ObjectCombo->AddItem("StaticMapObj");
	m_ObjectCombo->AddItem("TileObj");
	m_ObjectCombo->AddItem("Stage");
	m_ObjectCombo->AddItem("VerticalLampLight");
	m_ObjectCombo->AddItem("LopeTileObj");
	m_ObjectCombo->AddItem("NPCAdobis");
	m_ObjectCombo->AddItem("ZakumBody");
	m_ObjectCombo->AddItem("NPCAmon");
	m_ObjectCombo->AddItem("PukoMonster");
	m_ObjectCombo->AddItem("PunkoMonster");
	m_ObjectCombo->AddItem("CuzcoMonster");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectNameInput = AddWidget<CIMGUITextInput>("ObjectName", 100.f, 26.f);
	m_ObjectNameInput->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectCreateButton = AddWidget<CIMGUIButton>("Create Object", 120.f, 26.f);

	m_ObjectCreateButton->SetClickCallback(this, &CEditorMenu::ObjectCreateButton);


	// Component
	m_ComponentCombo = AddWidget<CIMGUIComboBox>("Select Component", 120.f, 26.f);

	m_ComponentCombo->SetHideName(true);
	m_ComponentCombo->AddItem("SpriteComponent");
	m_ComponentCombo->AddItem("StaticMeshComponent");
	m_ComponentCombo->AddItem("Box2DComponent");
	m_ComponentCombo->AddItem("CircleComponent");
	m_ComponentCombo->AddItem("PixelComponent");
	m_ComponentCombo->AddItem("CameraComponent");
	m_ComponentCombo->AddItem("WidgetComponent");
	m_ComponentCombo->AddItem("ParticleComponent");
	m_ComponentCombo->AddItem("TileMapComponent");
	m_ComponentCombo->AddItem("DragCollider");
	m_ComponentCombo->AddItem("SceneComponent");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentNameInput = AddWidget<CIMGUITextInput>("Component Name", 100.f, 26.f);
	m_ComponentNameInput->SetHideName(true);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentCreateButton = AddWidget<CIMGUIButton>("Create Component", 120.f, 26.f);

	m_ComponentCreateButton->SetClickCallback(this, &CEditorMenu::ComponentCreateButton);


	CIMGUIButton* SelectMapObjTexture = AddWidget<CIMGUIButton>("Select Object Texture", 150.f, 26.f);

	SelectMapObjTexture->SetClickCallback(this, &CEditorMenu::SelectObjTexture);

	//CIMGUIButton* SaveSceneButton = AddWidget<CIMGUIButton>("SaveSceneButton", 120.f, 30.f);

	//SaveSceneButton->SetClickCallback(this, &CEditorMenu::SaveScene);

	//Line = AddWidget<CIMGUISameLine>("Line");

	//CIMGUIButton* LoadSceneButton = AddWidget<CIMGUIButton>("LoadSceneButton", 120.f, 30.f);

	//LoadSceneButton->SetClickCallback(this, &CEditorMenu::LoadScene);

	return true;
}

void CEditorMenu::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	MyShowMenuFile();
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
	case CreateObject_Type::RadishMonster:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CRadishMonster>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::OnionMonster:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<COnionMonster>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::LowerClassBook:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLowerClassBook>(m_ObjectNameInput->GetTextMultibyte());
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
	case CreateObject_Type::LobbyBigLamp:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLobbyBigLamp>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::LobbySmallLamp:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLobbySmallLamp>(m_ObjectNameInput->GetTextMultibyte());
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
	case CreateObject_Type::CharacterSelectBackLight:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CCharacterSelectBackLight>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::StaticMapObj:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CStaticMapObj>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::TileMapObj:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CTileObject>(m_ObjectNameInput->GetTextMultibyte());
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
	case CreateObject_Type::VerticalLampLight:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CVerticalLampLight>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::LopeTileMapObj:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CLopeTileObject>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::NPCAdobis:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CNPCAdobis>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::ZakumBody:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CZakumBody>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::NPCAmon:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CNPCAmon>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Puko:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPuko>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Punko:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CPunko>(m_ObjectNameInput->GetTextMultibyte());
		break;
	case CreateObject_Type::Cuzco:
		CSceneManager::GetInst()->GetScene()->CreateGameObject<CCuzco>(m_ObjectNameInput->GetTextMultibyte());
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

	int	SelectIndex = m_ComponentCombo->GetSelectIndex();
	if (SelectIndex == -1)
		return;

	CSceneComponent* SelectComponent = Hierarchy->GetSelectComponent();

	switch ((SceneComponent_Type)SelectIndex)
	{
	case SceneComponent_Type::Sprite:
	{
		CSpriteComponent* Com = Obj->CreateComponent<CSpriteComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::StaticMesh:
	{
		CStaticMeshComponent* Com = Obj->CreateComponent<CStaticMeshComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::ColliderBox2D:
	{
		CColliderBox2D* Com = Obj->CreateComponent<CColliderBox2D>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::ColliderCircle:
	{
		CColliderCircle* Com = Obj->CreateComponent<CColliderCircle>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::ColliderPixel:
	{
		CColliderPixel* Com = Obj->CreateComponent<CColliderPixel>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::Camera:
	{
		CCameraComponent* Com = Obj->CreateComponent<CCameraComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::Widget:
	{
		CWidgetComponent* Com = Obj->CreateComponent<CWidgetComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::Particle:
	{
		CParticleComponent* Com = Obj->CreateComponent<CParticleComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}
	case SceneComponent_Type::TileMap:
	{
		CTileMapComponent* TileMap = Obj->CreateComponent<CTileMapComponent>(m_ComponentNameInput->GetTextMultibyte());

		if (TileMap == Obj->GetRootComponent())
		{
			CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("TileMap");

			TileMap->SetTileMaterial(Material);
			break;
		}

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(TileMap);
		else if (!SelectComponent && Obj->GetRootComponent() != TileMap)
			Obj->GetRootComponent()->AddChild(TileMap);
		else
			SelectComponent->AddChild(TileMap);

		CMaterial* Material = CSceneManager::GetInst()->GetScene()->GetResource()->FindMaterial("TileMap");

		TileMap->SetTileMaterial(Material);
	}
	break;
	case SceneComponent_Type::DragCollider:
	{
		CDragCollider* Com = Obj->CreateComponent<CDragCollider>(m_ComponentNameInput->GetTextMultibyte());

		if (Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
		{
			Obj->GetRootComponent()->AddChild(Com);
			Com->SetRelativePos(0.f, 0.f, 0.f);
		}
		else
		{
			SelectComponent->AddChild(Com);
			Com->SetRelativePos(0.f, 0.f, 0.f);
		}

		Com->Start();

		break;
	}
	case SceneComponent_Type::SceneComponent:
	{
		CSceneComponent* Com = Obj->CreateComponent<CSceneComponent>(m_ComponentNameInput->GetTextMultibyte());

		if(Com == Obj->GetRootComponent())
			break;

		if (!Obj->GetRootComponent())
			Obj->SetRootComponent(Com);
		else if (!SelectComponent && Obj->GetRootComponent() != Com)
			Obj->GetRootComponent()->AddChild(Com);
		else
			SelectComponent->AddChild(Com);
		break;
	}

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
	OpenFile.lpstrFilter = TEXT("葛电颇老\0*.*\0Scene File\0*.scn");
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
	OpenFile.lpstrFilter = TEXT("葛电颇老\0*.*\0Scene File\0*.scn");
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

		int Index = -1;

		Index = CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentList()->GetSelectIndex();

		if (Index == -1)
			return;

		std::string ComponentName = CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentList()->GetSelectItem();

		CSpriteComponent* SpriteComp = (CSpriteComponent*)SelectObject->FindComponent(ComponentName);

		if (!SpriteComp)
			return;

		//////////
		TCHAR   FilePath[MAX_PATH] = {};

		OPENFILENAME    OpenFile = {};

		OpenFile.lStructSize = sizeof(OPENFILENAME);
		OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
		OpenFile.lpstrFilter = TEXT("葛电颇老\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
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

			SpriteComp->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);

			SpriteComp->SetWorldScale((float)SpriteComp->GetMaterial()->GetTextureWidth(), (float)SpriteComp->GetMaterial()->GetTextureHeight(), 1.f);
		}
	}
}


void CEditorMenu::MyShowMenuFile()
{
	bool ShowAppMenuBar = false;

	if (ImGui::BeginMainMenuBar())
	{
		if (ImGui::BeginMenu("File"))
		{
			if (ImGui::MenuItem("Load Scene File"))
			{
				CObjectHierarchy* HierarchyWindow = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");
				CDetailWindow* DatailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

				if (HierarchyWindow)
				{
					HierarchyWindow->ClearHierarchyWindowInfo();
					HierarchyWindow->ClearComponentList();
					HierarchyWindow->ClearObjectList();
					HierarchyWindow->GetObjectList()->SetSelectIndex(-1);
					HierarchyWindow->GetComponentList()->SetSelectIndex(-1);

					if(HierarchyWindow->GetSelectObject())
						HierarchyWindow->GetSelectObject()->SetScene(nullptr);

					DatailWindow->ClearSelectObjectSequenceList();
				}

				LoadScene();
			}

			if (ImGui::MenuItem("Save Scene File"))
			{
				SaveScene();
			}

			ImGui::EndMenu();
		}

		if (ImGui::BeginMenu("Configuration"))
		{
			if (ImGui::BeginMenu("Font"))
			{
				if (ImGui::MenuItem("Default"))
				{
					CIMGUIManager::GetInst()->SetCurrentFont("Default");
				}

				if (ImGui::MenuItem("Bold"))
				{
					CIMGUIManager::GetInst()->SetCurrentFont("DefaultBold");
				}

				if (ImGui::MenuItem("Black"))
				{
					CIMGUIManager::GetInst()->SetCurrentFont("DefaultBlack");
				}

				ImGui::EndMenu();
			}

			if (ImGui::BeginMenu("Style"))
			{
				if (ImGui::MenuItem("Dark"))
				{
					ImGui::StyleColorsDark();
				}

				if (ImGui::MenuItem("Light"))
				{
					ImGui::StyleColorsLight();
				}

				if (ImGui::MenuItem("Classic"))
				{
					ImGui::StyleColorsClassic();
				}

				ImGui::EndMenu();
			}

			ImGui::EndMenu();
		}

		ImGui::EndMainMenuBar();
	}


}