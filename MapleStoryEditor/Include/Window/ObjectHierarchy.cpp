
#include "ObjectHierarchy.h"
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
#include "../Object/SpriteEditObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "../Object/DragObject.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"

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
	m_ComponentListWidget->SetSelectCallback(this, &CObjectHierarchy::SelectObject);

	return true;
}

void CObjectHierarchy::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
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
}

void CObjectHierarchy::SelectComponent(int Index, const char* Item)
{
}
