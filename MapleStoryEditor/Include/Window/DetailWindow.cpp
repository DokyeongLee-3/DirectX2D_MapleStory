
#include "DetailWindow.h"
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
#include "ObjectHierarchy.h"
#include "IMGUIManager.h"
#include "Render/RenderManager.h"
#include "SpriteWindow.h"

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Transform", 335.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Label = AddWidget<CIMGUILabel>("Position", 65.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosX = AddWidget<CIMGUITextInput>("PositionX", 45.f, 35.f);
	m_PosX->SetHideName(true);
	m_PosX->SetTextType(ImGuiText_Type::Float);
	m_PosX->SetCallback(this, &CDetailWindow::PositionXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosY = AddWidget<CIMGUITextInput>("PositionY", 45.f, 35.f);
	m_PosY->SetHideName(true);
	m_PosY->SetTextType(ImGuiText_Type::Float);
	m_PosY->SetCallback(this, &CDetailWindow::PositionYCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosZ = AddWidget<CIMGUITextInput>("PositionZ", 45.f, 35.f);
	m_PosZ->SetHideName(true);
	m_PosZ->SetTextType(ImGuiText_Type::Float);
	m_PosZ->SetCallback(this, &CDetailWindow::PositionZCallback);

	// Rotation InputText
	Label = AddWidget<CIMGUILabel>("Rotation", 65.f, 30.f);
	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);
	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotX = AddWidget<CIMGUITextInput>("RotationX", 45.f, 35.f);
	m_RotX->SetHideName(true);
	m_RotX->SetTextType(ImGuiText_Type::Float);
	m_RotX->SetCallback(this, &CDetailWindow::RotationXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotY = AddWidget<CIMGUITextInput>("RotationY", 45.f, 35.f);
	m_RotY->SetHideName(true);
	m_RotY->SetTextType(ImGuiText_Type::Float);
	m_RotY->SetCallback(this, &CDetailWindow::RotationYCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotZ = AddWidget<CIMGUITextInput>("RotationZ", 45.f, 35.f);
	m_RotZ->SetHideName(true);
	m_RotZ->SetTextType(ImGuiText_Type::Float);
	m_RotZ->SetCallback(this, &CDetailWindow::RotationZCallback);

	// Scaling
	Label = AddWidget<CIMGUILabel>("Scale", 65.f, 30.f);
	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);
	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScalingX = AddWidget<CIMGUITextInput>("ScaleX", 45.f, 35.f);
	m_ScalingX->SetHideName(true);
	m_ScalingX->SetTextType(ImGuiText_Type::Float);
	m_ScalingX->SetCallback(this, &CDetailWindow::ScalingXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(100, 100, 100);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScalingY = AddWidget<CIMGUITextInput>("ScaleY", 45.f, 35.f);
	m_ScalingY->SetHideName(true);
	m_ScalingY->SetTextType(ImGuiText_Type::Float);
	m_ScalingY->SetCallback(this, &CDetailWindow::ScalingYCallback);

	Label = AddWidget<CIMGUILabel>("Select Object Sequences", 335.f, 30.f);
	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	m_SelectObjectSequenceList = AddWidget<CIMGUIListBox>("SelectObjectSequenceList", 255.f, 350.f);
	m_SelectObjectSequenceList->SetHideName(true);
	m_SelectObjectSequenceList->SetPageItemCount(8);
	m_SelectObjectSequenceList->SetSelectCallback<CDetailWindow>(this, &CDetailWindow::ChangeSelectObjectAnimation);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_FlipButton = AddWidget<CIMGUIButton>("Flip", 75.f, 150.f);
	m_FlipButton->SetClickCallback(this, &CDetailWindow::FlipComponent);

	return true;
}

void CDetailWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CDetailWindow::PositionXCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	CIMGUIListBox* ComponentList = Hierarchy->GetComponentList();

	if (ComponentList->GetSelectIndex() == -1)
		return;

	CSceneComponent* Comp = nullptr;

	Comp = (CSceneComponent*)Obj->FindComponent(ComponentList->GetSelectItem());

	Vector3	OriginWorldPos = Comp->GetWorldPos();
	//Vector3	OriginRelativePos = Comp->GetRelativePos();

	//float Diff = m_PosX->GetValueFloat() - OriginWorldPos.x;

	//Comp->SetRelativePos(Vector3(OriginRelativePos.x + Diff, OriginRelativePos.y, OriginRelativePos.z));

	Vector3 NewWorldPos;
	NewWorldPos.x = m_PosX->GetValueFloat();

	Comp->SetWorldPos(NewWorldPos.x, OriginWorldPos.y, OriginWorldPos.z);
}

void CDetailWindow::PositionYCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	CIMGUIListBox* ComponentList = Hierarchy->GetComponentList();

	if (ComponentList->GetSelectIndex() == -1)
		return;

	CSceneComponent* Comp = nullptr;

	Comp = (CSceneComponent*)Obj->FindComponent(ComponentList->GetSelectItem());

	Vector3	OriginWorldPos = Comp->GetWorldPos();

	Vector3 NewWorldPos;
	NewWorldPos.y = m_PosY->GetValueFloat();

	Comp->SetWorldPos(OriginWorldPos.x, NewWorldPos.y, OriginWorldPos.z);
}

void CDetailWindow::PositionZCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = Hierarchy->GetSelectObject();

	if (!Obj)
		return;

	CSceneComponent* SelectComp = Hierarchy->GetSelectComponent();

	if (!SelectComp)
		return;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	//Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	Vector3	Pos = SelectComp->GetWorldPos();

	Pos.z = m_PosZ->GetValueFloat();

	std::string LayerName = SelectComp->GetLayerName();
	float LowerBound = CRenderManager::GetInst()->GetLayerLowerBoundZOrder(LayerName);
	float UpperBound = CRenderManager::GetInst()->GetLayerUpperBoundZOrder(LayerName);

	if (Pos.z > UpperBound)
		m_PosZ->SetValueFloat(UpperBound);
	if (Pos.z < LowerBound)
		m_PosZ->SetValueFloat(LowerBound);

	Pos.z = m_PosZ->GetValueFloat();

	SelectComp->SetWorldPos(Pos);
}

void CDetailWindow::RotationXCallback()
{
	// 2D에선 일단 막아두자
	return;

	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	Vector3	Rot = Obj->GetRootComponent()->GetWorldRot();

	Rot.x = m_RotX->GetValueFloat();

	Obj->SetWorldRotation(Rot);
}

void CDetailWindow::RotationYCallback()
{
	// 2D에선 일단 막아두자
	return;

	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	Vector3	Rot = Obj->GetRootComponent()->GetWorldRot();

	Rot.y = m_RotY->GetValueFloat();

	Obj->SetWorldRotation(Rot);
}

void CDetailWindow::RotationZCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	//CGameObject* Obj = nullptr;

	//CIMGUIListBox* ObjList = Hierarchy->GetObjectList();

	//if (ObjList->GetSelectIndex() == -1)
	//	return;

	//Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	//if (!Obj)
	//	return;

	CGameObject* SelectObject = Hierarchy->GetSelectObject();

	if (!SelectObject)
		return;

	CSceneComponent* SelectComponent = Hierarchy->GetSelectComponent();

	if (!SelectComponent)
		return;

	Vector3	Rot = SelectComponent->GetWorldRot();

	Rot.z = m_RotZ->GetValueFloat();

	SelectComponent->SetWorldRotation(Rot);
}

void CDetailWindow::ScalingXCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();
	CIMGUIListBox* CompList = Hierarchy->GetComponentList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	CComponent* Comp = Obj->FindComponent(CompList->GetSelectItem());

	if (!Comp)
		return;

	if (Comp->GetComponentType() != Component_Type::SceneComponent)
		return;

	Vector3	Scale = ((CSceneComponent*)Comp)->GetWorldScale();

	Scale.x = m_ScalingX->GetValueFloat();

	((CSceneComponent*)Comp)->SetWorldScale(Scale);
}

void CDetailWindow::ScalingYCallback()
{
	CObjectHierarchy* Hierarchy = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (!Hierarchy)
		return;

	CGameObject* Obj = nullptr;

	CIMGUIListBox* ObjList = Hierarchy->GetObjectList();
	CIMGUIListBox* CompList = Hierarchy->GetComponentList();

	if (ObjList->GetSelectIndex() == -1)
		return;

	Obj = CSceneManager::GetInst()->GetScene()->FindObject(ObjList->GetSelectItem());

	if (!Obj)
		return;

	CComponent* Comp = Obj->FindComponent(CompList->GetSelectItem());

	if (!Comp)
		return;

	if (Comp->GetComponentType() != Component_Type::SceneComponent)
		return;

	Vector3	Scale = ((CSceneComponent*)Comp)->GetWorldScale();

	Scale.y = m_ScalingY->GetValueFloat();

	((CSceneComponent*)Comp)->SetWorldScale(Scale);
}

void CDetailWindow::FlipComponent()
{
	CObjectHierarchy* Window = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (Window)
	{
		CSceneComponent* SelectComp = Window->GetSelectComponent();

		if (SelectComp)
		{
			if (SelectComp->GetTypeID() != typeid(CSpriteComponent).hash_code())
				return;

			((CSpriteComponent*)SelectComp)->Flip();
		}
	}
}

void CDetailWindow::ClearDetailWindowInfo()
{
	m_PosX->SetValueFloat(0.f);
	m_PosY->SetValueFloat(0.f);
	m_PosZ->SetValueFloat(0.f);

	m_RotX->SetValueFloat(0.f);
	m_RotY->SetValueFloat(0.f);
	m_RotZ->SetValueFloat(0.f);

	m_ScalingX->SetValueFloat(0.f);
	m_ScalingY->SetValueFloat(0.f);
}

void CDetailWindow::AddSelectObjectSequenceList(const std::vector<std::string>& vecSequence)
{
	size_t Count = vecSequence.size();

	for (size_t i = 0; i < Count; ++i)
	{
		m_SelectObjectSequenceList->AddItem(vecSequence[i]);
	}
}

void CDetailWindow::ClearSelectObjectSequenceList()
{
	m_SelectObjectSequenceList->Clear();
}

void CDetailWindow::ChangeSelectObjectAnimation(int Index, const char* Item)
{
	std::string SelectSequence = Item;

	CObjectHierarchy* HierarchyWindow = (CObjectHierarchy*)CIMGUIManager::GetInst()->FindIMGUIWindow("ObjectHierarchy");

	if (HierarchyWindow)
	{
		HierarchyWindow->ChangeSelectObjectAnimationSequence(SelectSequence);
		float PlayTime = HierarchyWindow->GetSequencePlayeTime(SelectSequence);

		CSpriteWindow* SpriteWindow = (CSpriteWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("SpriteWindow");

		if (SpriteWindow)
		{
			SpriteWindow->SetPlayTime(PlayTime);
		}
	}
}