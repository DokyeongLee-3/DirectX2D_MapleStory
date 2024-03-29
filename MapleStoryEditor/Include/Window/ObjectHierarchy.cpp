
#include "ObjectHierarchy.h"
#include "SpriteWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUIRadioButton.h"
#include "IMGUIManager.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "../Object/SpriteEditObject.h"
#include "../Object/DragObject.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "../EditorManager.h"
#include "DetailWindow.h"
#include "EditorMenu.h"
#include "TileMapWindow.h"
#include "Component/ColliderBox2D.h"
#include "Component/ColliderCircle.h"
#include "Component/ColliderPixel.h"
#include "Component/StaticMeshComponent.h"
#include "../Component/DragCollider.h"

#include <sstream>
#include "imgui_internal.h"

CObjectHierarchy::CObjectHierarchy()	:
	m_ProfileChangeButton(nullptr),
	m_ProfileCombo(nullptr)
{
}

CObjectHierarchy::~CObjectHierarchy()
{
}

void CObjectHierarchy::SetParentComponent(const char* Parent)
{
	m_ParentComponent->SetText(Parent);
}

void CObjectHierarchy::AddObjectList(const char* Name)
{
	m_ObjectListWidget->AddItem(Name);
}

void CObjectHierarchy::AddComponentList(const char* Name)
{
	m_ComponentListWidget->AddItem(Name);
}

bool CObjectHierarchy::Init()
{
	CIMGUIWindow::Init();

	m_ObjectListWidget = AddWidget<CIMGUIListBox>("ObjectList", 220.f, 400.f);

	m_ObjectListWidget->SetHideName(true);
	m_ObjectListWidget->SetPageItemCount(15);
	m_ObjectListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentListWidget = AddWidget<CIMGUIListBox>("ComponentList", 220.f, 400.f);

	m_ComponentListWidget->SetHideName(true);
	m_ComponentListWidget->SetPageItemCount(15);
	m_ComponentListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectComponent);

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("", 250.f, 10.f);
	Label->SetColor(0, 0, 0, 0);

	m_ObjectDeleteButton = AddWidget<CIMGUIButton>("Delete Object", 100.f, 30.f);

	m_ObjectDeleteButton->SetClickCallback(this, &CObjectHierarchy::DeleteObjectButtonCallback);

	Line = AddWidget<CIMGUISameLine>("Line");
	//Line->SetOffsetX(120.f);

	m_ComponentDeleteButton = AddWidget<CIMGUIButton>("Delete Component", 110.f, 30.f);

	m_ComponentDeleteButton->SetClickCallback(this, &CObjectHierarchy::DeleteComponentButtonCallback);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(230.f);

	CIMGUILabel* ParentLabel = AddWidget<CIMGUILabel>("Parent Component : ", 120.f, 30.f);
	//ParentLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
	ParentLabel->SetColor(0, 0, 150, 0);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);

	m_ParentComponent = AddWidget<CIMGUIText>("Parent", 90.f, 30.f);



	// Layer
	m_LayerCombo = AddWidget<CIMGUIComboBox>("Select Layer", 100.f, 30.f);

	m_LayerCombo->SetHideName(true);
	//m_LayerCombo->AddItem("Default");
	m_LayerCombo->AddItem("Stage");
	m_LayerCombo->AddItem("MapObjBackMost");
	m_LayerCombo->AddItem("MapObjBack");
	m_LayerCombo->AddItem("MapObjMiddle");
	m_LayerCombo->AddItem("MapObjFront");
	m_LayerCombo->AddItem("MovingObjFront");
	m_LayerCombo->AddItem("CoveringMapObj");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_LayerChangeButton = AddWidget<CIMGUIButton>("Change Layer", 100.f, 30.f);

	m_LayerChangeButton->SetClickCallback<CObjectHierarchy>(this, &CObjectHierarchy::LayerChangeCallback);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);
	Line->SetOffsetX(230.f);

	m_ObjectLayerLabel = AddWidget<CIMGUILabel>("Layer : ", 50.f, 30.f);
	//m_ObjectLayerLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
	m_ObjectLayerLabel->SetColor(0, 0, 150, 0);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectLayer = AddWidget<CIMGUIText>("ObjectLayer", 100.f, 30.f);



	m_ProfileCombo = AddWidget<CIMGUIComboBox>("Select Profile", 100.f, 30.f);
	m_ProfileCombo->SetHideName(true);
	m_ProfileCombo->AddItem("Object");
	m_ProfileCombo->AddItem("Player");
	m_ProfileCombo->AddItem("Monster");
	m_ProfileCombo->AddItem("MapCollider");
	m_ProfileCombo->AddItem("DragCollider");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ProfileChangeButton = AddWidget<CIMGUIButton>("Change Profile", 100.f, 30.f);

	m_ProfileChangeButton->SetClickCallback<CObjectHierarchy>(this, &CObjectHierarchy::ProfileChangeCallback);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);
	Line->SetOffsetX(230.f);

	Label = AddWidget<CIMGUILabel>("Collision Profile : ", 110.f, 30.f);
	//ProfileLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
	Label->SetColor(0, 0, 150, 0);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);

	m_Profile = AddWidget<CIMGUIText>("Profile", 100.f, 30.f);

	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	int Select = m_ObjectListWidget->GetSelectIndex();

	if (Select != -1)
	{
		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(m_ObjectListWidget->GetSelectItem());
		CSceneComponent* Root = SelectObject->GetRootComponent();

		if (Root)
		{
			if (Root->GetTypeID() == typeid(CSpriteComponent).hash_code())
			{
				CSpriteComponent* SpriteRoot = (CSpriteComponent*)(SelectObject->GetRootComponent());

				if (SpriteRoot->GetAnimationInstance() && SpriteRoot->GetCurrentAnimation())
				{
					int CurrentFrame = SpriteRoot->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

					std::stringstream ss;
					ss << CurrentFrame;

					CEditorManager::GetInst()->GetSpriteWindow()->GetCurrentFrameText()->SetText(ss.str().c_str());
				}
			}
		}
	}
}

void CObjectHierarchy::SelectObject(int Index, const char* Item)
{
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindObject(Item);

	if (!Object)
		return;

	if (m_SelectObject && m_SelectObject == Object)
		return;

	m_SelectObject = Object;

	// Object와 Component를 선택해놓았을때 기존에 선택한 오브젝트가 아닌 다른 오브젝트 선택하는 순간, 선택되어있던 Component를 해제하고
	// 이전에 Component를 선택해서 Window에 남아있던 Component의 transform정보, 부모Component, Layer, Collision Profile, ZOrder,
	// (이전에 선택한 Component가 TileMapComponent였다면) TileMapWindow가 갖고 있는 TileMapComponent까지 싹다 초기화해준다
	m_ComponentListWidget->SetSelectIndex(-1);
	CTileMapWindow* TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");
	TileMapWindow->SetTileMap(nullptr);
	CDetailWindow* DetailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");
	DetailWindow->ClearDetailWindowInfo();
	ClearHierarchyWindowInfo();

	m_ComponentListWidget->Clear();


	CSceneComponent* Root = Object->GetRootComponent();

	if (!Root)
		return;

	std::vector<FindComponentName>	vecNames;

	Object->GetAllSceneComponentsName(vecNames);

	//m_ObjectLayer->SetText(Object->GetRootComponent()->GetLayerName().c_str());

	size_t	Size = vecNames.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_ComponentListWidget->AddItem(vecNames[i].Name);
	}

	if (DetailWindow)
	{
		if (m_SelectObject->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
		{
			CSpriteComponent* RootComp = (CSpriteComponent*)m_SelectObject->GetRootComponent();
			CAnimationSequence2DInstance* Instance = RootComp->GetAnimationInstance();

			if (Instance)
			{
				std::vector<std::string> vecSequeceName;

				DetailWindow->ClearSelectObjectSequenceList();
				Instance->GetAllAnimationSequenceName(vecSequeceName);

				DetailWindow->AddSelectObjectSequenceList(vecSequeceName);
			}

			else
				DetailWindow->ClearSelectObjectSequenceList();


			std::string MeshName = RootComp->GetMeshName();
			std::string MaterialName = RootComp->GetMaterial()->GetTexture()->GetName();

			CMaterial* Material = RootComp->GetMaterial();
			CTexture* Tex = nullptr;

			if (RootComp->GetAnimationInstance())
			{
				AnimationFrameData Data = RootComp->GetAnimationInstance()->GetCurrentAnimation()->GetFrameData(0);

				if (Material->GetTexture())
					Tex = Material->GetTexture();
				else
					Tex = CResourceManager::GetInst()->FindTexture("DefaultUI");

				DetailWindow->SetMaterialImage(Tex);
				DetailWindow->SetMaterialImageStart(Data.Start);
				DetailWindow->SetMaterialImageEnd(Data.Start + Data.Size);

				DetailWindow->SetMeshImage("SpriteMeshImage", TEXT("RectMeshImage.png"));
			}

			else
			{
				if (Material)
				{
					if (Material->GetTexture())
						Tex = Material->GetTexture();
					else
						Tex = CResourceManager::GetInst()->FindTexture("DefaultUI");
				}

				else
				{
					Tex = CResourceManager::GetInst()->FindTexture("DefaultUI");
				}

				DetailWindow->SetMaterialImageStart(Vector2(0.f, 0.f));

				unsigned int Width = Tex->GetWidth();
				unsigned int Height = Tex->GetHeight();
				DetailWindow->SetMaterialImageEnd(Vector2((float)Width, (float)Height));
				DetailWindow->SetMaterialImage(Tex);
				DetailWindow->SetMeshImage("SpriteMeshImage", TEXT("RectMeshImage.png"));
			}

			DetailWindow->SetMeshName(MeshName);
			DetailWindow->SetMaterialName(MaterialName);
		}

		if (m_SelectObject->GetRootComponent()->GetTypeID() == typeid(CTileMapComponent).hash_code())
		{
			CTileMapComponent* RootComp = (CTileMapComponent*)m_SelectObject->GetRootComponent();

			std::string MeshName = RootComp->GetMeshName();

			CMaterial* Material = RootComp->GetTileMaterial();
			std::string MaterialName;
			
			if (Material && Material->GetTexture())
				MaterialName = Material->GetTexture()->GetName();
			else
				MaterialName = "NoName";

			DetailWindow->SetMeshName(MeshName);
			DetailWindow->SetMaterialName(MaterialName);
			DetailWindow->SetMeshImage("SpriteMeshImage", TEXT("RectMeshImage.png"));

			int TileCountX = RootComp->GetTileCountX();
			int TileCountY = RootComp->GetTileCountY();

			Vector2 Start = RootComp->GetTile(0)->GetFrameStart();
			Vector3 TileSize = RootComp->GetTileSize();


			DetailWindow->SetMaterialImage(Material->GetTexture());
			DetailWindow->SetMaterialImageStart(Start);
			DetailWindow->SetMaterialImageEnd(Start + Vector2(TileSize.x, TileSize.y));
		}
	}
}

void CObjectHierarchy::SelectComponent(int Index, const char* Item)
{
	if (!m_SelectObject)
		return;

	// 선택된 컴포넌트를 저장해둔다.
	m_SelectComponent = (CSceneComponent*)m_SelectObject->FindComponent(Item);

	CSceneComponent* Parent = m_SelectComponent->GetParent();

	if (Parent)
		m_ParentComponent->SetText(Parent->GetName().c_str());
	else
		m_ParentComponent->SetText("No Parent(Root)");

	Vector3 WorldPos = m_SelectComponent->GetWorldPos();
	Vector3 WorldRotation = m_SelectComponent->GetWorldRot();
	Vector3 WorldScale = m_SelectComponent->GetWorldScale();

	CEditorManager::GetInst()->GetDetailWindow()->GetPosXInput()->SetValueFloat(WorldPos.x);
	CEditorManager::GetInst()->GetDetailWindow()->GetPosYInput()->SetValueFloat(WorldPos.y);
	CEditorManager::GetInst()->GetDetailWindow()->GetPosZInput()->SetValueFloat(WorldPos.z);

	CEditorManager::GetInst()->GetDetailWindow()->GetRotXInput()->SetValueFloat(WorldRotation.x);
	CEditorManager::GetInst()->GetDetailWindow()->GetRotYInput()->SetValueFloat(WorldRotation.y);
	CEditorManager::GetInst()->GetDetailWindow()->GetRotZInput()->SetValueFloat(WorldRotation.z);

	CEditorManager::GetInst()->GetDetailWindow()->GetScaleXInput()->SetValueFloat(WorldScale.x);
	CEditorManager::GetInst()->GetDetailWindow()->GetScaleYInput()->SetValueFloat(WorldScale.y);

	CTileMapWindow* TileMapWindow = (CTileMapWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("TileMapWindow");
	CDetailWindow* DetailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

	if (TileMapWindow)
	{
		if (m_SelectComponent->CheckType<CTileMapComponent>())
		{

			if (CEditorManager::GetInst()->GetEditMode() != EditMode::TileMap)
			{
				CSpriteWindow* SpriteWindow = (CSpriteWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("SpriteWindow");

				CIMGUIRadioButton* Radio = SpriteWindow->GetTileMapEditRadioButton();
				Radio->SetActive(0, true);
				Radio->CallRadioButtonCallback(0);

				/*MessageBox(0, TEXT("Please Check Map Edit Mode first"), TEXT("WARNING"), MB_OK);
				CEngine::GetInst()->Exit();*/
			}

			TileMapWindow->SetTileMap((CTileMapComponent*)m_SelectComponent.Get());

		}

		else
			TileMapWindow->SetTileMap(nullptr);
	}

	if (m_SelectComponent->GetTypeID() == typeid(CColliderBox2D).hash_code() ||
		m_SelectComponent->GetTypeID() == typeid(CColliderCircle).hash_code() ||
		m_SelectComponent->GetTypeID() == typeid(CColliderPixel).hash_code())
	{
		m_Profile->SetText(((CColliderComponent*)m_SelectComponent.Get())->GetCollisionProfile()->Name.c_str());
	}


	// ComponentList에서 Component 선택하면 그 Component가 속한 Object의 RootComponent의 ZOrder, Layer를 보여준다
	CSceneComponent* Root = m_SelectComponent->GetGameObject()->GetRootComponent();

	if (Root->GetTypeID() == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Sprite = (CSpriteComponent*)(Root);

		if (Sprite->GetAnimationInstance() && Sprite->GetCurrentAnimation() && Sprite->GetCurrentAnimation()->GetFrameCount() > 0)
		{
			float PlayTime = Sprite->GetCurrentAnimation()->GetPlayTime();
			CEditorManager::GetInst()->GetSpriteWindow()->SetPlayTime(PlayTime);
		}
	}

	m_ObjectLayer->SetText(m_SelectComponent->GetLayerName().c_str());


	/*	int Idx = m_LayerCombo->GetSelectIndex();

		if (Idx == -1)
			return;

		const std::string& LayerName = m_LayerCombo->GetSelectItem();

		if (LayerName == "Default")
		{
			((CSceneComponent*)Com)->SetLayerName("Default");
		}

		else if (LayerName == "MapObjBack")
		{
			((CSceneComponent*)Com)->SetLayerName("MapObjBack");
		}

		else if (LayerName == "MapObjMiddle")
		{
			((CSceneComponent*)Com)->SetLayerName("MapObjMiddle");
		}

		else if (LayerName == "MapObjFront")
		{
			((CSceneComponent*)Com)->SetLayerName("MapObjFront");
		}*/
}

void CObjectHierarchy::LayerChangeCallback()
{
	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(m_SelectObject->GetName());

	if (!Obj)
		return;

	CObjectHierarchy* Window = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	CComponent* Comp = Window->GetSelectComponent();

	if (!Comp)
		return;

	if (Comp->GetComponentType() == Component_Type::SceneComponent)
	{
		if (m_LayerCombo->GetSelectIndex() == -1)
			return;

		const std::string& LayerName = m_LayerCombo->GetSelectItem();

		//if (LayerName == ((CSceneComponent*)Comp)->GetLayerName())
		//	return;

		((CSceneComponent*)Comp)->SetLayerName(LayerName);

		m_ObjectLayer->SetText(Obj->GetRootComponent()->GetLayerName().c_str());
	}
}

void CObjectHierarchy::DeleteObjectButtonCallback()
{
	int SelectObjIdx = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectIndex();

	if (SelectObjIdx == -1)
		return;

	const std::string& ObjName = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectItem();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

	if (!Obj)
		return;

	m_ObjectListWidget->DeleteItem(SelectObjIdx);

	int Count = m_ComponentListWidget->GetItemCount();

	for (int i = 0; i < Count; ++i)
	{
		m_ComponentListWidget->DeleteItem(0);
	}

	Obj->Destroy();

	m_ObjectListWidget->SetSelectIndex(-1);
	m_ComponentListWidget->SetSelectIndex(-1);

	CDetailWindow* DetailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

	if (DetailWindow)
		DetailWindow->ClearSelectObjectSequenceList();
		
}

void CObjectHierarchy::DeleteComponentButtonCallback()
{
	if (!m_SelectObject)
		return;

	if (!m_SelectComponent)
		return;

	if (m_SelectComponent == m_SelectObject->GetRootComponent())
	{
		m_SelectObject->ClearSceneComponents();
		m_ComponentListWidget->Clear();

	}

	else
	{
		m_SelectComponent->ClearChild();
		CSceneComponent* Component = (CSceneComponent*)m_SelectObject->FindComponent(m_SelectComponent->GetName());
		m_SelectObject->DeleteSceneComponent(Component);

		int SelectIndex = m_ComponentListWidget->GetSelectIndex();
		m_ComponentListWidget->DeleteItem(SelectIndex);
	}

	m_ComponentListWidget->SetSelectIndex(-1);
}


void CObjectHierarchy::ProfileChangeCallback()
{
	m_SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(m_SelectObject->GetName());

	if (!m_SelectObject)
		return;

	if (m_ComponentListWidget->GetSelectIndex() == -1)
		return;

	std::string Component = m_ComponentListWidget->GetSelectItem();

	CComponent* Comp = m_SelectObject->FindComponent(Component);

	if (!Comp)
		return;

	if (Comp->GetTypeID() == typeid(CColliderBox2D).hash_code() || Comp->GetTypeID() == typeid(CColliderCircle).hash_code() ||
		Comp->GetTypeID() == typeid(CColliderPixel).hash_code())
	{
		std::string Profile = m_ProfileCombo->GetSelectItem();
		m_Profile->SetText(Profile.c_str());
		((CColliderComponent*)Comp)->SetCollisionProfile(Profile);
	}
}

void CObjectHierarchy::ClearObjectList()
{
	m_ObjectListWidget->Clear();
}

void CObjectHierarchy::ClearComponentList()
{
	m_ComponentListWidget->Clear();
}

void CObjectHierarchy::ClearHierarchyWindowInfo()
{
	m_ObjectLayer->SetText("");
	//m_LayerCombo->SetSelectIndex(-1);
	m_Profile->SetText("");
	m_SelectComponent = nullptr;
	m_ParentComponent->SetText("");
}

void CObjectHierarchy::ChangeSelectObjectAnimationSequence(const std::string& SequenceName)
{
	if (m_SelectObject)
	{
		if (m_SelectObject->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
		{
			CSpriteComponent* Root = (CSpriteComponent*)m_SelectObject->GetRootComponent();

			Root->ChangeAnimation(SequenceName);
		}
	}
}

float CObjectHierarchy::GetSequencePlayeTime(const std::string& SequenceName)
{
	if (m_SelectObject)
	{
		if (m_SelectObject->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
		{
			CSpriteComponent* Root = (CSpriteComponent*)m_SelectObject->GetRootComponent();

			CAnimationSequence2DData* Data = Root->GetAnimationInstance()->FindAnimation(SequenceName);

			if (Data)
				return Data->GetPlayTime();
		}
	}

	return 1.f;
}
