
#include "ConfigurationWindow.h"
#include "IMGUITree.h"
#include "IMGUIRadioButton.h"
#include "SpriteWindow.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "IMGUIManager.h"
#include "Scene/SceneManager.h"

CConfigurationWindow::CConfigurationWindow()
{
}

CConfigurationWindow::~CConfigurationWindow()
{
}

bool CConfigurationWindow::Init()
{

    CIMGUIWindow::Init();

    //Tree±¸Çö

    CIMGUITree* Tree = AddWidget<CIMGUITree>("Tree");
    Node* NewNode = Tree->AddChildNode("", "Configuration");
    Tree->SetSize(60.f, 60.f);

    NewNode = Tree->AddChildNode("Configuration", "Style");

    NewNode->Callback = std::bind(&CConfigurationWindow::StyleCallback, this);

    NewNode = Tree->AddChildNode("Configuration", "EditMode");

    CIMGUIRadioButton* Radio = Tree->AddNodeWidget<CIMGUIRadioButton>("EditMode", 50.f, 50.f);
    Radio->AddText<CConfigurationWindow>("Map Edit Mode", this, &CConfigurationWindow::MapEditMode);
    Radio->AddText<CConfigurationWindow>("Sprite Edit Mode", this, &CConfigurationWindow::SpriteEditMode);

	return true;
}

void CConfigurationWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);
}

void CConfigurationWindow::StyleCallback()
{
    MyShowStyleEditor();
}

void CConfigurationWindow::MyShowStyleEditor(ImGuiStyle* ref)
{
    // You can pass in a reference ImGuiStyle structure to compare to, revert to and save to
    // (without a reference style pointer, we will use one compared locally as a reference)
    ImGuiStyle& style = ImGui::GetStyle();
    static ImGuiStyle ref_saved_style;

    // Default to using internal storage as reference
    static bool init = true;
    if (init && ref == NULL)
        ref_saved_style = style;
    init = false;
    if (ref == NULL)
        ref = &ref_saved_style;

    ImGui::PushItemWidth(ImGui::GetWindowWidth() * 0.50f);

    if (ImGui::ShowStyleSelector("Colors##Selector"))
        ref_saved_style = style;
    ImGui::ShowFontSelector("Fonts##Selector");
}


void CConfigurationWindow::MapEditMode()
{
    int a = 3;
}

void CConfigurationWindow::SpriteEditMode()
{
    CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

    CSpriteWindow* SpriteWindow = (CSpriteWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("SpriteWindow");

    CSpriteEditObject* SpriteObj = SpriteWindow->GetSpriteObject();

    if (!SpriteObj)
    {
        SpriteObj = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
        SpriteWindow->SetSpriteObject(SpriteObj);
    }


}
