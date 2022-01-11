
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

CDetailWindow::CDetailWindow()
{
}

CDetailWindow::~CDetailWindow()
{
}

bool CDetailWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("Transform", 350.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Label = AddWidget<CIMGUILabel>("Position", 60.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosX = AddWidget<CIMGUITextInput>("PositionX", 100.f, 30.f);
	m_PosX->SetHideName(true);
	m_PosX->SetTextType(ImGuiText_Type::Int);
	m_PosX->SetCallback(this, &CDetailWindow::PositionXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosY = AddWidget<CIMGUITextInput>("PositionY", 100.f, 30.f);
	m_PosY->SetHideName(true);
	m_PosY->SetTextType(ImGuiText_Type::Int);
	m_PosY->SetCallback(this, &CDetailWindow::PositionYCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_PosZ = AddWidget<CIMGUITextInput>("PositionZ", 100.f, 30.f);
	m_PosZ->SetHideName(true);
	m_PosZ->SetTextType(ImGuiText_Type::Int);
	m_PosZ->SetCallback(this, &CDetailWindow::PositionZCallback);

	// Rotation InputText
	Label = AddWidget<CIMGUILabel>("Rotation", 60.f, 30.f);
	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);
	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotX = AddWidget<CIMGUITextInput>("RotationX", 100.f, 30.f);
	m_RotX->SetHideName(true);
	m_RotX->SetTextType(ImGuiText_Type::Int);
	m_RotX->SetCallback(this, &CDetailWindow::RotationXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotY = AddWidget<CIMGUITextInput>("RotationY", 100.f, 30.f);
	m_RotY->SetHideName(true);
	m_RotY->SetTextType(ImGuiText_Type::Int);
	m_RotY->SetCallback(this, &CDetailWindow::RotationYCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Z", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_RotZ = AddWidget<CIMGUITextInput>("RotationZ", 100.f, 30.f);
	m_RotZ->SetHideName(true);
	m_RotZ->SetTextType(ImGuiText_Type::Int);
	m_RotZ->SetCallback(this, &CDetailWindow::RotationZCallback);

	// Scaling
	Label = AddWidget<CIMGUILabel>("Scale", 60.f, 30.f);
	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("X", 30.f, 30.f);
	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScalingX = AddWidget<CIMGUITextInput>("ScaleX", 100.f, 30.f);
	m_ScalingX->SetHideName(true);
	m_ScalingX->SetTextType(ImGuiText_Type::Int);
	m_ScalingX->SetCallback(this, &CDetailWindow::ScalingXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("Y", 30.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_ScalingY = AddWidget<CIMGUITextInput>("ScaleY", 100.f, 30.f);
	m_ScalingY->SetHideName(true);
	m_ScalingY->SetTextType(ImGuiText_Type::Int);
	m_ScalingY->SetCallback(this, &CDetailWindow::ScalingYCallback);

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
	NewWorldPos.x = (float)m_PosX->GetValueInt();

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
	NewWorldPos.y = (float)m_PosY->GetValueInt();

	Comp->SetWorldPos(OriginWorldPos.x, NewWorldPos.y, OriginWorldPos.z);
}

void CDetailWindow::PositionZCallback()
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

	Vector3	Pos = Obj->GetRootComponent()->GetWorldPos();

	Pos.z = (float)m_PosZ->GetValueInt();

	Obj->GetRootComponent()->SetWorldPos(Pos);
}

void CDetailWindow::RotationXCallback()
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

	Vector3	Rot = Obj->GetRootComponent()->GetWorldRot();

	Rot.x = (float)m_RotX->GetValueInt();

	Obj->SetWorldRotation(Rot);
}

void CDetailWindow::RotationYCallback()
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

	Vector3	Rot = Obj->GetRootComponent()->GetWorldRot();

	Rot.y = (float)m_RotY->GetValueInt();

	Obj->SetWorldRotation(Rot);
}

void CDetailWindow::RotationZCallback()
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

	Vector3	Rot = Obj->GetRootComponent()->GetWorldRot();

	Rot.z = (float)m_RotZ->GetValueInt();

	Obj->SetWorldRotation(Rot);
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

	Scale.x = (float)m_ScalingX->GetValueInt();

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

	Scale.y = (float)m_ScalingY->GetValueInt();

	((CSceneComponent*)Comp)->SetWorldScale(Scale);
}
