#include "IMGUITestWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUIImage.h"
#include "IMGUIRadioButton.h"
#include "IMGUICheckBox.h"

CIMGUITestWindow::CIMGUITestWindow()
{
}

CIMGUITestWindow::~CIMGUITestWindow()
{
}

bool CIMGUITestWindow::Init()
{
    CIMGUIWindow::Init();

    CIMGUIButton* Button = AddWidget<CIMGUIButton>("Button1");

    Button->SetClickCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton1);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(100.f);
    Line->SetSpacing(100.f);

    Button = AddWidget<CIMGUIButton>("Button2", 100.f, 30.f);

    Button->SetClickCallback<CIMGUITestWindow>(this, &CIMGUITestWindow::ClickButton2);

    CIMGUILabel* Label = AddWidget<CIMGUILabel>("Test", 100.f, 30.f);

    Label->SetColor(255, 0, 0);
    Label->SetAlign(0.5f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    CIMGUIText* Text = AddWidget<CIMGUIText>("Text", 100.f, 30.f);
    Text->SetText("ÇÑ±ÛÆÐÄ¡ ½ÇÆÐ");
    Text->SetColor(255, 255, 0);

    CIMGUITextInput* TextInput = AddWidget<CIMGUITextInput>("TextInput", 200.f, 30.f);
    TextInput->SetHideName(true);
    //TextInput->EnablePassword(true);


    Line = AddWidget<CIMGUISameLine>("Line");
    CIMGUIRadioButton* Radio = AddWidget<CIMGUIRadioButton>("Radio", 100.f, 100.f);
    Radio->AddText<CIMGUITestWindow>("Â¥Àå¸é", this, &CIMGUITestWindow::RadioCallback1);
    Radio->AddText<CIMGUITestWindow>("Â«»Í", this, &CIMGUITestWindow::RadioCallback2);
    Radio->AddText<CIMGUITestWindow>("ÅÁ¼öÀ°", this, &CIMGUITestWindow::RadioCallback3);


    CIMGUIListBox* ListBox = AddWidget<CIMGUIListBox>("ListBox", 200.f, 30.f);
    ListBox->SetHideName(true);

    ListBox->AddItem("Æ¼¸ð");
    ListBox->AddItem("°¡ºØÀÌ");
    ListBox->AddItem("¿ìºØÀÌ");
    ListBox->AddItem("¿ìºØÀÌ1");

    Line = AddWidget<CIMGUISameLine>("Line");

    CIMGUICheckBox* CheckBox = AddWidget<CIMGUICheckBox>("CheckBox", 100.f, 100.f);
    CheckBox->SetText<CIMGUITestWindow>("ÁØºñµÆ´Â°¡?", this, &CIMGUITestWindow::CheckBoxCallback1);


    CIMGUIComboBox* ComboBox = AddWidget<CIMGUIComboBox>("ComboBox", 200.f, 30.f);
    ComboBox->SetHideName(true);

    ComboBox->AddItem("Æ¼¸ð");
    ComboBox->AddItem("°¡ºØÀÌ");
    ComboBox->AddItem("¿ìºØÀÌ");

    for (int i = 0; i < 100; ++i)
    {
        ComboBox->AddItem("¿ìºØÀÌ1");
    }

    CIMGUIImage* Image = AddWidget<CIMGUIImage>("Image", 200.f, 200.f);

    Image->SetTexture("TeemoTest", TEXT("Teemo.jpg"));
    Image->SetImageStart(0.f, 0.f);
    Image->SetImageEnd(1215.f, 717.f);
    Image->SetTint(255, 0, 0);

    return true;
}

void CIMGUITestWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);
}

void CIMGUITestWindow::ClickButton1()
{
    MessageBox(0, TEXT("aa"), TEXT("aa"), MB_OK);
}

void CIMGUITestWindow::ClickButton2()
{
    MessageBox(0, TEXT("bb"), TEXT("bb"), MB_OK);
}

void CIMGUITestWindow::ListBoxCallback1(int Index, const char* Name)
{
}

void CIMGUITestWindow::RadioCallback1()
{
    MessageBox(0, TEXT("Â¥Àå¸é ¸Ô°í½Í´Ù"), TEXT("Â¥Àå¸é ¸Ô°í½Í´Ù"), MB_OK);
}

void CIMGUITestWindow::RadioCallback2()
{
    MessageBox(0, TEXT("Â«»Í ¸Ô°í½Í´Ù"), TEXT("Â«»Í ¸Ô°í½Í´Ù"), MB_OK);
}

void CIMGUITestWindow::RadioCallback3()
{
    MessageBox(0, TEXT("ÅÁ¼öÀ° ¸Ô°í½Í´Ù"), TEXT("ÅÁ¼öÀ° ¸Ô°í½Í´Ù"), MB_OK);
}

void CIMGUITestWindow::CheckBoxCallback1()
{
    MessageBox(0, TEXT("½ÃÀÛÇÏÁö"), TEXT("½ÃÀÛÇÏÁö"), MB_OK);
}