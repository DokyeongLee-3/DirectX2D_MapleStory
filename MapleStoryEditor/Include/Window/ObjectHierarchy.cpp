
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

#include <sstream>

CObjectHierarchy::CObjectHierarchy()
{
}

CObjectHierarchy::~CObjectHierarchy()
{
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


	// Layer
	m_LayerCombo = AddWidget<CIMGUIComboBox>("Select Layer", 130.f, 30.f);

	m_LayerCombo->SetHideName(true);
	m_LayerCombo->AddItem("Default");
	m_LayerCombo->AddItem("Stage");
	m_LayerCombo->AddItem("MapObjBack");
	m_LayerCombo->AddItem("MapObjFront");
	m_LayerCombo->AddItem("MovingObjFront");

	Line = AddWidget<CIMGUISameLine>("Line");

	m_LayerChangeButton = AddWidget<CIMGUIButton>("Change Layer", 100.f, 30.f);

	m_LayerChangeButton->SetClickCallback<CObjectHierarchy>(this, &CObjectHierarchy::LayerChangeCallback);

	Line = AddWidget<CIMGUISameLine>("Line", 300.f);

	m_ObjectLayerLabel = AddWidget<CIMGUILabel>("Object Layer : ", 90.f, 30.f);
	m_ObjectLayerLabel->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ObjectLayer = AddWidget<CIMGUIText>("ObjectLayer", 100.f, 30.f);


	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	int Select = m_ObjectListWidget->GetSelectIndex();

	if (Select != -1)
	{
		CGameObject* SelectObject = CSceneManager::GetInst()->GetScene()->FindObject(m_ObjectListWidget->GetSelectItem());

		if (SelectObject->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
		{
			CSpriteComponent* Root = (CSpriteComponent*)(SelectObject->GetRootComponent());

			if (Root->GetAnimationInstance() && Root->GetCurrentAnimation())
			{
				int CurrentFrame = Root->GetAnimationInstance()->GetCurrentAnimation()->GetCurrentFrame();

				std::stringstream ss;
				ss << CurrentFrame;

				CEditorManager::GetInst()->GetSpriteWindow()->GetCurrentFrameText()->SetText(ss.str().c_str());
			}
		}
	}
}

void CObjectHierarchy::SelectObject(int Index, const char* Item)
{
	CGameObject* Object = CSceneManager::GetInst()->GetScene()->FindObject(Item);

	if (!Object)
		return;

	std::vector<FindComponentName>	vecNames;

	Object->GetAllSceneComponentsName(vecNames);

	m_ComponentListWidget->Clear();

	size_t	Size = vecNames.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_ComponentListWidget->AddItem(vecNames[i].Name);
	}

	if (Object->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
	{
		CSpriteComponent* Sprite = (CSpriteComponent*)(Object->GetRootComponent());

		if (Sprite->GetAnimationInstance() && Sprite->GetCurrentAnimation() && Sprite->GetCurrentAnimation()->GetFrameCount() > 0)
		{
			float PlayTime = Sprite->GetCurrentAnimation()->GetPlayTime();
			CEditorManager::GetInst()->GetSpriteWindow()->SetPlayTime(PlayTime);
		}


		m_ObjectLayer->SetText(Object->GetRootComponent()->GetLayerName().c_str());
	}
}

void CObjectHierarchy::SelectComponent(int Index, const char* Item)
{
	const std::string& ObjName = m_ObjectListWidget->GetSelectItem();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

	if (!Obj)
		return;

	CComponent* Com = Obj->FindComponent(Item);

	if (Com->GetComponentType() == Component_Type::SceneComponent)
	{
		Vector3 WorldPos = ((CSceneComponent*)Com)->GetWorldPos();
		Vector3 WorldRotation = ((CSceneComponent*)Com)->GetWorldRot();
		Vector3 WorldScale = ((CSceneComponent*)Com)->GetWorldScale();

		CEditorManager::GetInst()->GetDetailWindow()->GetPosXInput()->SetValueInt((int)WorldPos.x);
		CEditorManager::GetInst()->GetDetailWindow()->GetPosYInput()->SetValueInt((int)WorldPos.y);
		CEditorManager::GetInst()->GetDetailWindow()->GetPosZInput()->SetValueInt((int)WorldPos.z);

		CEditorManager::GetInst()->GetDetailWindow()->GetRotXInput()->SetValueInt((int)WorldRotation.x);
		CEditorManager::GetInst()->GetDetailWindow()->GetRotYInput()->SetValueInt((int)WorldRotation.y);
		CEditorManager::GetInst()->GetDetailWindow()->GetRotZInput()->SetValueInt((int)WorldRotation.z);

		CEditorManager::GetInst()->GetDetailWindow()->GetScaleXInput()->SetValueInt((int)WorldScale.x);
		CEditorManager::GetInst()->GetDetailWindow()->GetScaleYInput()->SetValueInt((int)WorldScale.y);


		int Idx = m_LayerCombo->GetSelectIndex();

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
		}
	}
}

void CObjectHierarchy::LayerChangeCallback()
{
	int SelectObjIdx = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectIndex();

	if (SelectObjIdx == -1)
		return;

	const std::string& ObjName = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->GetSelectItem();

	CGameObject* Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjName);

	if (!Obj)
		return;

	CComponent* Comp = Obj->GetRootComponent();

	if (Comp->GetComponentType() == Component_Type::SceneComponent)
	{
		if (m_LayerCombo->GetSelectIndex() == -1)
			return;

		const std::string& LayerName = m_LayerCombo->GetSelectItem();

		if (LayerName == ((CSceneComponent*)Comp)->GetLayerName())
			return;

		((CSceneComponent*)Comp)->SetLayerName(LayerName);

		m_ObjectLayer->SetText(Obj->GetRootComponent()->GetLayerName().c_str());
	}
}
