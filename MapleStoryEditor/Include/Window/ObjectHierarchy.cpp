
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

	m_ObjectListWidget = AddWidget<CIMGUIListBox>("ObjectList", 300.f, 500.f);

	m_ObjectListWidget->SetHideName(true);
	m_ObjectListWidget->SetPageItemCount(15);
	m_ObjectListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_ComponentListWidget = AddWidget<CIMGUIListBox>("ComponentList", 300.f, 500.f);

	m_ComponentListWidget->SetHideName(true);
	m_ComponentListWidget->SetPageItemCount(15);
	m_ComponentListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectComponent);


	m_ObjectDeleteButton = AddWidget<CIMGUIButton>("Delete Object", 120.f, 30.f);

	m_ObjectDeleteButton->SetClickCallback(this, &CObjectHierarchy::DeleteObjectButtonCallback);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(140.f);

	m_ComponentDeleteButton = AddWidget<CIMGUIButton>("Delete Component", 120.f, 30.f);

	m_ComponentDeleteButton->SetClickCallback(this, &CObjectHierarchy::DeleteComponentButtonCallback);

	Line = AddWidget<CIMGUISameLine>("Line");
	Line->SetOffsetX(320.f);

	CIMGUILabel* ParentLabel = AddWidget<CIMGUILabel>("Parent Component : ", 120.f, 30.f);
	ParentLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);

	m_ParentComponent = AddWidget<CIMGUIText>("Parent", 90.f, 30.f);



	// Layer
	m_LayerCombo = AddWidget<CIMGUIComboBox>("Select Layer", 130.f, 30.f);

	m_LayerCombo->SetHideName(true);
	m_LayerCombo->AddItem("Default");
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
	Line->SetOffsetX(320.f);

	m_ObjectLayerLabel = AddWidget<CIMGUILabel>("Layer : ", 50.f, 30.f);
	m_ObjectLayerLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectLayer = AddWidget<CIMGUIText>("ObjectLayer", 100.f, 30.f);


	m_ZOrder = AddWidget<CIMGUITextInput>("ZOrder", 80.f, 30.f);
	m_ZOrder->SetHideName(true);
	m_ZOrder->SetText("");
	m_ZOrder->SetSize(100.f, 20.f);
	m_ZOrder->SetTextType(ImGuiText_Type::Int);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ZOrderChange = AddWidget<CIMGUIButton>("ZOrder Change", 120.f, 30.f);
	m_ZOrderChange->SetClickCallback(this, &CObjectHierarchy::ZOrderChangeCallback);




	m_ProfileCombo = AddWidget<CIMGUIComboBox>("Select Profile", 130.f, 30.f);
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
	Line->SetOffsetX(320.f);

	CIMGUILabel* ProfileLabel = AddWidget<CIMGUILabel>("Collision Profile : ", 110.f, 30.f);
	ProfileLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

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
	
	/*if (Size > 0)
	{
		if (Root->GetTypeID() == typeid(CSpriteComponent).hash_code())
		{
			CSpriteComponent* Sprite = (CSpriteComponent*)(Root);

			if (Sprite->GetAnimationInstance() && Sprite->GetCurrentAnimation() && Sprite->GetCurrentAnimation()->GetFrameCount() > 0)
			{
				float PlayTime = Sprite->GetCurrentAnimation()->GetPlayTime();
				CEditorManager::GetInst()->GetSpriteWindow()->SetPlayTime(PlayTime);
			}

			m_ObjectLayer->SetText(Object->GetRootComponent()->GetLayerName().c_str());
			m_ZOrder->SetValueInt(Sprite->GetZOrder());
		}

		else if (Object->GetRootComponent()->GetTypeID() == typeid(CSceneComponent).hash_code())
		{
			CSceneComponent* Comp = (CSceneComponent*)(Object->GetRootComponent());
			m_ZOrder->SetValueInt(Comp->GetZOrder());
		}
	}*/
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

	if (TileMapWindow)
	{
		if (m_SelectComponent->CheckType<CTileMapComponent>())
		{

			if (CEditorManager::GetInst()->GetEditMode() != EditMode::TileMap)
			{
				CSpriteWindow* SpriteWindow = (CSpriteWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("SpriteWindow");

				CIMGUIRadioButton* Radio = SpriteWindow->GetRadioButton();
				Radio->SetActive((int)EditMode::TileMap, true);
				Radio->CallRadioButtonCallback((int)EditMode::TileMap);

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
	m_ZOrder->SetValueInt(m_SelectComponent->GetZOrder());




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

	CComponent* Comp = Obj->GetRootComponent();

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

void CObjectHierarchy::ZOrderChangeCallback()
{
	if (m_ObjectListWidget->GetSelectIndex() == -1)
		return;

	if (m_ComponentListWidget->GetSelectIndex() == -1)
		return;

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(m_ObjectListWidget->GetSelectItem());

	if (!Obj)
		return;

	CComponent* Comp = Obj->FindComponent(m_ComponentListWidget->GetSelectItem());

	if (Comp)
	{
		if (Comp->GetTypeID() == typeid(CSceneComponent).hash_code() || Comp->GetTypeID() == typeid(CSpriteComponent).hash_code()
			|| Comp->GetTypeID() == typeid(CTileMapComponent).hash_code() || Comp->GetTypeID() == typeid(CColliderBox2D).hash_code()
			|| Comp->GetTypeID() == typeid(CColliderCircle).hash_code() || Comp->GetTypeID() == typeid(CColliderPixel).hash_code()
			|| Comp->GetTypeID() == typeid(CDragCollider).hash_code())
		{
			((CSceneComponent*)Comp)->SetZOrder(m_ZOrder->GetValueInt());
		}
	}
}

void CObjectHierarchy::ProfileChangeCallback()
{
	m_SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(m_SelectObject->GetName());

	if (!m_SelectObject)
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

void CObjectHierarchy::ClearHierarchyWindowInfo()
{
	m_ObjectLayer->SetText("");
	//m_LayerCombo->SetSelectIndex(-1);
	m_ZOrder->SetValueInt(0);
	m_Profile->SetText("");
	m_SelectComponent = nullptr;
	m_ParentComponent->SetText("");
}
