#include "SpriteWindow.h"
#include "IMGUIManager.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "IMGUICheckBox.h"
#include "IMGUIRadioButton.h"
#include "IMGUIImage.h"
#include "IMGUITree.h"
#include "Device.h"
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "../Object/SpriteEditObject.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "../Object/Pivot.h"
#include "../Object/DragObject.h"
#include "ObjectHierarchy.h"

#include <sstream>

CSpriteWindow::CSpriteWindow() :
    m_SpriteObject(nullptr),
    m_Sprite(nullptr),
    m_SpriteFrame(nullptr),
    m_AnimInstance(nullptr),
    m_FrameStartPosX(nullptr),
    m_FrameEndPosX(nullptr),
    m_FrameStartPosY(nullptr),
    m_FrameEndPosY(nullptr),
    m_LoopCheckBox(nullptr),
    m_ReverseCheckBox(nullptr),
    m_AnimPlayScale(nullptr),
    m_AnimPlayTime(nullptr),
    m_LoadFileName(nullptr),
    m_DragPivotXPos(nullptr),
    m_DragPivotYPos(nullptr),
    m_CurrentFrameText(nullptr)
    //m_EditorAnimationLoadObject(nullptr)
{
}

CSpriteWindow::~CSpriteWindow()
{
    SAFE_DELETE(m_AnimInstance);
}

void CSpriteWindow::SetPlayTime(float PlayTime)
{
    m_AnimPlayTime->SetValueFloat(PlayTime);
}

void CSpriteWindow::SetSpriteObject(CSpriteEditObject* Obj)
{
    m_SpriteObject = Obj;
}

bool CSpriteWindow::Init()
{
    CIMGUIWindow::Init();

    CIMGUITree* Tree = AddWidget<CIMGUITree>("Tree");
    Node* NewNode = Tree->AddChildNode("", "Configuration");

    NewNode = Tree->AddChildNode("Configuration", "Style");

    NewNode->Callback = std::bind(&CSpriteWindow::StyleCallback, this);

    NewNode = Tree->AddChildNode("Configuration", "EditMode");

    CIMGUIRadioButton* Radio = Tree->AddNodeWidget<CIMGUIRadioButton>("EditMode", 100.f, 100.f);
    Radio->AddText<CSpriteWindow>("Scene Edit Mode", this, &CSpriteWindow::ObjectArrangeButton);
    Radio->AddText<CSpriteWindow>("Sprite Edit Mode", this, &CSpriteWindow::SpriteEditButton);
    Radio->AddText<CSpriteWindow>("Map Edit Mode", this, &CSpriteWindow::MapEditButton);

    CIMGUILabel* Label = AddWidget<CIMGUILabel>("", 600.f, 50.f);
    Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);

    CIMGUIButton* Button = AddWidget<CIMGUIButton>("Load a Texture");

    Button->SetSize(120.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadTextureButton);

    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

    m_LoadFileName = AddWidget<CIMGUITextInput>("LoadFileName");
    m_LoadFileName->SetHideName(true);
    m_LoadFileName->SetText("");
    m_LoadFileName->SetSize(200.f, 20.f);
    m_LoadFileName->ReadOnly(true);

    Label = AddWidget<CIMGUILabel>("Animation Sequence", 150.f, 30.f);

    Label->SetColorFloat(0.28f, 0.28f, 0.28f, 0.8f);
    Label->SetAlign(0.5f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_AnimName = AddWidget<CIMGUITextInput>("AnimNameInput", 80.f, 30.f);
    m_AnimName->SetHideName(true);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(420.f);

    Label = AddWidget<CIMGUILabel>("Frames for Sequence", 150.f, 30.f);

    Label->SetColorFloat(0.28f, 0.28f, 0.28f, 0.8f);
    Label->SetAlign(0.5f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_AnimFrameName = AddWidget<CIMGUITextInput>("AnimFrameNameInput", 80.f, 30.f);
    m_AnimFrameName->SetHideName(true);

    m_AnimationList = AddWidget<CIMGUIListBox>("AnimationList", 150.f, 300.f);
    m_AnimationList->SetHideName(true);
    m_AnimationList->SetPageItemCount(6);
    m_AnimationList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimation);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Add", 50.f, 40.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Save", 50.f, 40.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SaveSequence);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Load", 50.f, 40.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadSequence);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Delete", 50.f, 40.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteSequenceButton);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(420.f);

    m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 150.f, 300.f);
    m_AnimationFrameList->SetHideName(true);
    m_AnimationFrameList->SetPageItemCount(6);
    m_AnimationFrameList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimationFrame);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Add##", 50.f, 40.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Delete##", 50.f, 40.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteFrameButton);




    m_FrameStartPosX = AddWidget<CIMGUITextInput>("FrameStartPosX", 120.f, 120.f);
    m_FrameStartPosX->SetInt(0);
    m_FrameStartPosX->SetTextType(ImGuiText_Type::Int);
    m_FrameStartPosX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataStartX);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_FrameStartPosY = AddWidget<CIMGUITextInput>("FrameStartPosY", 120.f, 120.f);
    m_FrameStartPosY->SetInt(0);
    m_FrameStartPosY->SetTextType(ImGuiText_Type::Int);
    m_FrameStartPosY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataStartY);
    //FrameStartPos->ReadOnly(true);

    m_FrameEndPosX = AddWidget<CIMGUITextInput>("FrameEndPosX", 120.f, 120.f);
    m_FrameEndPosX->SetInt(100);
    m_FrameEndPosX->SetTextType(ImGuiText_Type::Int);
    m_FrameEndPosX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataEndX);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetSpacing(16.f);

    m_FrameEndPosY = AddWidget<CIMGUITextInput>("FrameEndPosY", 120.f, 120.f);
    m_FrameEndPosY->SetInt(100);
    m_FrameEndPosY->SetTextType(ImGuiText_Type::Int);
    m_FrameEndPosY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataEndY);
    //m_FrameEndPosY->ReadOnly(true);

    m_AnimPlayTime = AddWidget<CIMGUITextInput>("AnimPlayTime");
    m_AnimPlayTime->SetTextType(ImGuiText_Type::Float);
    m_AnimPlayTime->SetFloat(1.f);
    m_AnimPlayTime->SetCallback<CSpriteWindow>(this, &CSpriteWindow::ChangePlayTime);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_AnimPlayScale = AddWidget<CIMGUITextInput>("AnimPlayScale");
    m_AnimPlayScale->SetTextType(ImGuiText_Type::Float);
    m_AnimPlayScale->SetFloat(1.f);
    //m_AnimPlayScale->SetCallback<CSpriteWindow>(this, &CSpriteWindow::ChangePlayScale);

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
    Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    Line->SetOffsetX(500.f);

    Button = AddWidget<CIMGUIButton>("Play", 30.f, 20.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::PlayAnimation);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Stop", 30.f, 20.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::StopAnimation);



    m_LoopCheckBox = AddWidget<CIMGUICheckBox>("LoopCheckBox");
    m_LoopCheckBox->SetText<CIMGUICheckBox>("Loop", nullptr, nullptr);
    m_LoopCheckBox->SetCheckCallback<CSpriteWindow>(this, &CSpriteWindow::ChangeLoop);
    m_LoopCheckBox->SetUnCheckCallback<CSpriteWindow>(this, &CSpriteWindow::ChangeNoLoop);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_ReverseCheckBox = AddWidget<CIMGUICheckBox>("ReverseCheckBox");
    m_ReverseCheckBox->SetText<CIMGUICheckBox>("Reverse", nullptr, nullptr);
    m_ReverseCheckBox->SetCheckCallback<CSpriteWindow>(this, &CSpriteWindow::ChangeReverse);
    m_ReverseCheckBox->SetUnCheckCallback<CSpriteWindow>(this, &CSpriteWindow::ChangeNoReverse);

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("CenterXPos", 50.f, 20.f);
    Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_DragPivotXPos = AddWidget<CIMGUIText>("CenterXPos");

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("CenterYPos", 50.f, 20.f);
    Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_DragPivotYPos = AddWidget<CIMGUIText>("CenterYPos");

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("CurrentFrame", 90.f, 20.f);
    Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_CurrentFrameText = AddWidget<CIMGUIText>("CurrentFrame");



    m_Sprite = AddWidget<CIMGUIImage>("SpriteOrigin", 200.f, 200.f);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_SpriteFrame = AddWidget<CIMGUIImage>("SpriteFrame", 200.f, 200.f);

    Line = AddWidget<CIMGUISameLine>("Line");


    // SpriteComponent���ؼ� CreateAnimationInstance�� �ϸ� Instance�� Scene��
    // Set�Ǿ� �ְ����� �� ��쿣 �ƴ϶� Instance��
    // SpriteCompoentŸ���� m_Owner�� ���� �����ִ� m_Scene�� ����
    // m_AnimInstance�� �̿��ؼ� m_SpriteFrame�� �ִϸ��̼� Play��ư ������
    // �ش� �������� �ִϸ��̼��� �����ϰ� ����µ�, �̰� ���̴��� �׸��°� �ƴ϶�
    // m_AnimInstance�� ������۵� nullptr�̴� 
    m_AnimInstance = new CAnimationSequence2DInstance;

    m_AnimInstance->Init();

    m_AnimInstance->Start();

    m_AnimInstance->Stop();

    // m_EditorAnimationLoadObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CAnimationLoadObject>("AnimationLoadObject");

    return true;
}

void CSpriteWindow::Update(float DeltaTime)
{
    CIMGUIWindow::Update(DeltaTime);

    DeltaTime *= m_AnimPlayScale->GetValueFloat();

    m_AnimInstance->Update(DeltaTime);

    if (m_AnimInstance->IsPlay())
    {
        CAnimationSequence2DData* AnimData = m_AnimInstance->GetCurrentAnimation();

        if (AnimData && AnimData->GetAnimationSequence()->GetFrameCount() > 0)
        {
            int Frame = AnimData->GetCurrentFrame();

            AnimationFrameData  FrameData = AnimData->GetAnimationSequence()->GetFrameData(Frame);

            m_SpriteFrame->SetImageStart(FrameData.Start.x, FrameData.Start.y);

            m_SpriteFrame->SetImageEnd(FrameData.Start.x + FrameData.Size.x, FrameData.Start.y + FrameData.Size.y);

            m_SpriteFrame->SetSize(FrameData.Size);
        }
    }

    //static bool Open = false;

    //ImGui::ShowDemoWindow(&Open);

}

void CSpriteWindow::LoadTextureButton()
{
    EditMode Mode = CEditorManager::GetInst()->GetEditMode();

    if (Mode != EditMode::Sprite)
        return;

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
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

        // ���� �ȿ��� ImageEnd�� ����ش�(ImageStart�� (0,0) �״��)
        m_Sprite->SetTextureFullPath(ConvertFileName, FilePath);

        if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
        {
            m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)ConstantBuffer_Shader_Type::Pixel,
                ConvertFileName, FilePath);

            m_SpriteObject->GetSpriteComponent()->SetWorldScale((float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth(),
                (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight(), 1.f);
        }

        else if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
        {
            
        }

        strcat_s(ConvertFileName, ConvertFileExt);

        m_LoadFileName->SetText(ConvertFileName);
    }

}

void CSpriteWindow::SpriteEditButton()
{
    CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

    if (!m_SpriteObject)
    {
        m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
    }
}

void CSpriteWindow::MapEditButton()
{
    CEditorManager::GetInst()->SetEditMode(EditMode::Map);
}

void CSpriteWindow::ObjectArrangeButton()
{
    CEditorManager::GetInst()->SetEditMode(EditMode::Scene);
}

void CSpriteWindow::AddAnimationButton()
{
    if (m_AnimName->Empty())
        return;

    const char* Text = m_AnimName->GetTextMultibyte();

    if (m_AnimationList->CheckItem(Text))
        return;

    if (m_AnimInstance->IsPlay())
        m_AnimInstance->Stop();

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    if (!Resource->CreateAnimationSequence2D(Text, m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()))
        return;

    m_AnimationList->AddItem(Text);

    float PlayTime = m_AnimPlayTime->GetValueFloat();
    float PlayScale = m_AnimPlayScale->GetValueFloat();
    bool Loop = m_LoopCheckBox->IsCheck();
    bool Reverse = m_ReverseCheckBox->IsCheck();

    m_AnimInstance->AddAnimation(Text, Text, Loop, PlayTime, PlayScale, Reverse);

}

void CSpriteWindow::AddAnimationFrameButton()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Anim = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    Vector2 Size;
    Vector2 FrameStartPos = Vector2((float)(m_FrameStartPosX->GetValueInt()), (float)(m_FrameStartPosY->GetValueInt()));

    Size.x = (float)(m_FrameEndPosX->GetValueInt() - m_FrameStartPosX->GetValueInt());
    Size.y = (float)(m_FrameEndPosY->GetValueInt() - m_FrameStartPosY->GetValueInt());

    // �̹� m_FrameStartPosX, m_FrameStartPosY, m_FrameEndPosX, m_FrameEndPosY�� �巡����ġ�� ����ϴµ� �巡�� ��ġ��
    // ����� �� �̹������� ��ǥ�� ��ȯ�س����� 
    Anim->AddFrame(FrameStartPos, Size);

    int FrameCount = Anim->GetFrameCount() - 1;

    char    FrameName[32] = {};
    sprintf_s(FrameName, "%d", FrameCount);

    m_AnimationFrameList->AddItem(FrameName);

    m_SpriteFrame->SetTexture(m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture());

    Vector2 ImageStartPos = DragObj->GetStartPos();
    Vector2 ImageEndPos = DragObj->GetEndPos();

    ImageStartPos.x -= m_SpriteObject->GetWorldPos().x;
    ImageStartPos.y -= m_SpriteObject->GetWorldPos().y;

    ImageEndPos.x -= m_SpriteObject->GetWorldPos().x;
    ImageEndPos.y -= m_SpriteObject->GetWorldPos().y;

    ImageEndPos.y = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - ImageEndPos.y;
    ImageStartPos.y = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight() - ImageStartPos.y;

    Vector2 StartPos, EndPos;
    StartPos.x = ImageStartPos.x < ImageEndPos.x ? ImageStartPos.x : ImageEndPos.x;
    StartPos.y = ImageStartPos.y < ImageEndPos.y ? ImageStartPos.y : ImageEndPos.y;

    EndPos.x = ImageStartPos.x > ImageEndPos.x ? ImageStartPos.x : ImageEndPos.x;
    EndPos.y = ImageStartPos.y > ImageEndPos.y ? ImageStartPos.y : ImageEndPos.y;

    m_SpriteFrame->SetImageStart(StartPos.x, StartPos.y);

    m_SpriteFrame->SetImageEnd(EndPos.x, EndPos.y);

    Vector2 SpriteFrameSize;
    SpriteFrameSize.x = abs(EndPos.x - StartPos.x);
    SpriteFrameSize.y = abs(EndPos.y - StartPos.y);


    m_SpriteFrame->SetSize(SpriteFrameSize);
}

void CSpriteWindow::SelectAnimationFrame(int Index, const char* Item)
{
    if (Index == -1)
        return;

    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Anim = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    AnimationFrameData  Data = Anim->GetFrameData(Index);

    m_SpriteFrame->SetImageStart(Data.Start.x, Data.Start.y);

    m_SpriteFrame->SetImageEnd(Data.Start.x + Data.Size.x, Data.Start.y + Data.Size.y);

    m_SpriteFrame->SetSize(Data.Size);

    m_FrameStartPosX->SetInt((int)Data.Start.x);
    m_FrameStartPosY->SetInt((int)Data.Start.y);
    m_FrameEndPosX->SetInt((int)(Data.Start.x + Data.Size.x));
    m_FrameEndPosY->SetInt((int)(Data.Start.y + Data.Size.y));

    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    Vector2 StartPos, EndPos;

    float   ImageHeight = (float)m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight();

    StartPos.x = Data.Start.x + m_SpriteObject->GetWorldPos().x;
    StartPos.y = ImageHeight - Data.Start.y + m_SpriteObject->GetWorldPos().y;

    EndPos.x = StartPos.x + Data.Size.x;
    EndPos.y = StartPos.y - Data.Size.y;

    DragObj->SetStartPos(StartPos);
    DragObj->SetEndPos(EndPos);

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    Pivot->SetWorldPos((StartPos.x + EndPos.x) / 2.f, (StartPos.y + EndPos.y) / 2.f, 0.f);

    // ���� �߽��� ��ġ ����
    float FrameCenterX = (m_FrameStartPosX->GetValueInt() + m_FrameEndPosX->GetValueInt()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueInt() + m_FrameEndPosY->GetValueInt()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}

void CSpriteWindow::SelectAnimation(int Index, const char* Item)
{
    m_AnimInstance->SetCurrentAnimation(Item);

    int FrameCount = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameCount();

    m_AnimationFrameList->Clear();

    for (int i = 0; i < FrameCount; ++i)
    {
        char    FrameName[32] = {};
        sprintf_s(FrameName, "%d", i);
        m_AnimationFrameList->AddItem(FrameName);

        m_SpriteFrame->SetTexture(m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetTexture());

        if (FrameCount > 0)
        {
            AnimationFrameData  Data = m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->GetFrameData(i);

            m_SpriteFrame->SetImageStart(Data.Start.x, Data.Start.y);

            m_SpriteFrame->SetImageEnd(Data.Start.x + Data.Size.x, Data.Start.y + Data.Size.y);
        }
    }
}

void CSpriteWindow::SaveSequence()
{
    int SelectAnimIndex = m_AnimationList->GetSelectIndex();
    if (SelectAnimIndex == -1)
        return;

    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0Sequence File\0*.sqc");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    // GetOpenFileNameó�� ��ȭ���ڿ��� ������ ������ ��θ� ���� �� �ִٴ� ������ ������
    // GetSaveFileName�� ��ȭ���ڰ� "�ٸ� �̸����� ����" �̶�� Ÿ��Ʋ�� ������ 
    // GetOpenFileName�� "����"��� Ÿ��Ʋ�� ������
    if (GetSaveFileName(&OpenFile) != 0)
    {
        FILE* pFile = nullptr;

        char    FullPath[MAX_PATH] = {};

        int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

        m_AnimInstance->GetCurrentAnimation()->GetAnimationSequence()->SaveFullPath(FullPath);
    }
}

void CSpriteWindow::LoadSequence()
{
    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0Sequence File\0*.sqc");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        FILE* pFile = nullptr;

        char    FullPath[MAX_PATH] = {};

        int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

        CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

        std::string	SequenceName;
        // �ᱹ LoadSequence�� ���Ͽ��� ������ �о ����������(CAnimationManager����) ���� �ϳ� ����°�
        Resource->LoadSequence2DFullPath(SequenceName, FullPath);

        CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(SequenceName);

        if (!Sequence)
            return;

        //bool Loop = m_LoopCheckBox->IsCheck();

        m_AnimInstance->AddAnimation(SequenceName, SequenceName, true, 1.f);

        m_AnimationList->AddItem(Sequence->GetName());
    }
}

void CSpriteWindow::SaveAnimation()
{
    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0Instance File\0*.ins");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    // GetOpenFileNameó�� ��ȭ���ڿ��� ������ ������ ��θ� ���� �� �ִٴ� ������ ������
    // GetSaveFileName�� ��ȭ���ڰ� "�ٸ� �̸����� ����" �̶�� Ÿ��Ʋ�� ������ 
    // GetOpenFileName�� "����"��� Ÿ��Ʋ�� ������
    if (GetSaveFileName(&OpenFile) != 0)
    {
        char    FullPath[MAX_PATH] = {};

        int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

        //m_AnimInstance->Save(FullPath);
    }
}

void CSpriteWindow::LoadAnimation()
{
    TCHAR   FilePath[MAX_PATH] = {};

    OPENFILENAME    OpenFile = {};

    OpenFile.lStructSize = sizeof(OPENFILENAME);
    OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
    OpenFile.lpstrFilter = TEXT("�������\0*.*\0Instance File\0*.ins");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    if (GetOpenFileName(&OpenFile) != 0)
    {
        FILE* pFile = nullptr;

        char    FullPath[MAX_PATH] = {};

        int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
        WideCharToMultiByte(CP_ACP, 0, FilePath, -1, FullPath, Length, 0, 0);

        //m_EditorAnimationLoadObject->LoadAnimation2DInstance("AnimationLoadComponent", FullPath);
    }

   /* CSpriteComponent* Comp = (CSpriteComponent*)m_EditorAnimationLoadObject->GetRootComponent();

    int Count = Comp->GetAnimationInstance()->GetAnimationCount();

    for (int i = 0; i < Count; ++i)
    {
        CAnimationSequence2DData* Data = Comp->GetAnimationInstance()->GetAnimationSequence2DData(i);

        std::string SequenceName = Data->GetAnimationSequence()->GetName();

        m_AnimInstance->AddAnimation(SequenceName, Data->GetName(), Data->IsLoop(),
            Data->GetPlayTime(), Data->GetPlayScale(), Data->IsReverse());

        if (m_AnimationList->CheckItem(SequenceName.c_str()))
            return;

        CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

        if (!Resource->CreateAnimationSequence2D(SequenceName, Data->GetAnimationSequence()->GetTexture()))
            return;

        m_AnimationList->AddItem(SequenceName.c_str());
    }*/
}

void CSpriteWindow::AdjustFrameDataStartX()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    int NewFrameStartX = m_FrameStartPosX->GetValueInt();
    int NewFrameStartY = m_FrameStartPosY->GetValueInt();
    int NewFrameEndX = m_FrameEndPosX->GetValueInt();
    int NewFrameEndY = m_FrameEndPosY->GetValueInt();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);

    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    // �巡�� ���� ����
    DragObj->SetStartPos(Vector2(m_SpriteObject->GetWorldPos().x + (float)NewFrameStartX, DragObj->GetStartPos().y));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewXPivotPos = (DragObj->GetStartPos().x + DragObj->GetEndPos().x) / 2.f;

    Pivot->SetWorldPos(NewXPivotPos, Pivot->GetWorldPos().y, Pivot->GetWorldPos().z);

    // Center ������ ��ġ ����
    float FrameCenterX = (m_FrameStartPosX->GetValueInt() + m_FrameEndPosX->GetValueInt()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueInt() + m_FrameEndPosY->GetValueInt()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}

void CSpriteWindow::AdjustFrameDataStartY()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    int NewFrameStartX = m_FrameStartPosX->GetValueInt();
    int NewFrameStartY = m_FrameStartPosY->GetValueInt();
    int NewFrameEndX = m_FrameEndPosX->GetValueInt();
    int NewFrameEndY = m_FrameEndPosY->GetValueInt();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    unsigned int TexHeight = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight();

    // �巡�� ���� ����
    DragObj->SetStartPos(Vector2(DragObj->GetStartPos().x, m_SpriteObject->GetWorldPos().y + TexHeight - (float)NewFrameStartY));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewYPivotPos = (DragObj->GetStartPos().y + DragObj->GetEndPos().y) / 2.f;

    Pivot->SetWorldPos(Pivot->GetWorldPos().x, NewYPivotPos, Pivot->GetWorldPos().z);

    // Center ������ ��ġ ����
    float FrameCenterX = (m_FrameStartPosX->GetValueInt() + m_FrameEndPosX->GetValueInt()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueInt() + m_FrameEndPosY->GetValueInt()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}

void CSpriteWindow::AdjustFrameDataEndX()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    int NewFrameStartX = m_FrameStartPosX->GetValueInt();
    int NewFrameStartY = m_FrameStartPosY->GetValueInt();
    int NewFrameEndX = m_FrameEndPosX->GetValueInt();
    int NewFrameEndY = m_FrameEndPosY->GetValueInt();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    // �巡�� ���� ����
    DragObj->SetEndPos(Vector2(m_SpriteObject->GetWorldPos().x + (float)NewFrameEndX, DragObj->GetEndPos().y));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewXPivotPos = (DragObj->GetStartPos().x + DragObj->GetEndPos().x)/2.f;

    Pivot->SetWorldPos(NewXPivotPos, Pivot->GetWorldPos().y, Pivot->GetWorldPos().z);

    // Center ������ ��ġ ����
    float FrameCenterX = (m_FrameStartPosX->GetValueInt() + m_FrameEndPosX->GetValueInt()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueInt() + m_FrameEndPosY->GetValueInt()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}

void CSpriteWindow::AdjustFrameDataEndY()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    int NewFrameStartX = m_FrameStartPosX->GetValueInt();
    int NewFrameStartY = m_FrameStartPosY->GetValueInt();
    int NewFrameEndX = m_FrameEndPosX->GetValueInt();
    int NewFrameEndY = m_FrameEndPosY->GetValueInt();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    unsigned int TexHeight = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight();

    // �巡�� ���� ����
    DragObj->SetEndPos(Vector2(DragObj->GetEndPos().x, m_SpriteObject->GetWorldPos().y + TexHeight - (float)NewFrameEndY));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewYPivotPos = (DragObj->GetStartPos().y + DragObj->GetEndPos().y) / 2.f;

    Pivot->SetWorldPos(Pivot->GetWorldPos().x, NewYPivotPos, Pivot->GetWorldPos().z);

    // Center ������ ��ġ ����
    float FrameCenterX = (m_FrameStartPosX->GetValueInt() + m_FrameEndPosX->GetValueInt()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueInt() + m_FrameEndPosY->GetValueInt()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}

void CSpriteWindow::ChangeLoop()
{
    if (m_AnimationList->GetSelectIndex() == -1)
        return;

    std::string SelectSequence = m_AnimationList->GetSelectItem();

    CAnimationSequence2DData* Data = m_AnimInstance->FindAnimation(SelectSequence);

    if (!Data)
        return;

    if (m_LoopCheckBox->IsCheck())
        Data->SetLoop(true);
}

void CSpriteWindow::ChangeNoLoop()
{
    if (m_AnimationList->GetSelectIndex() == -1)
        return;

    std::string SelectSequence = m_AnimationList->GetSelectItem();

    CAnimationSequence2DData* Data = m_AnimInstance->FindAnimation(SelectSequence);

    if (!Data)
        return;

    if (!m_LoopCheckBox->IsCheck())
        Data->SetLoop(false);
}

void CSpriteWindow::ChangeReverse()
{
    if (m_AnimationList->GetSelectIndex() == -1)
        return;

    std::string SelectSequence = m_AnimationList->GetSelectItem();

    CAnimationSequence2DData* Data = m_AnimInstance->FindAnimation(SelectSequence);

    if (!Data)
        return;

    if (m_ReverseCheckBox->IsCheck())
        Data->SetReverse(true);
}

void CSpriteWindow::ChangeNoReverse()
{
    if (m_AnimationList->GetSelectIndex() == -1)
        return;

    std::string SelectSequence = m_AnimationList->GetSelectItem();

    CAnimationSequence2DData* Data = m_AnimInstance->FindAnimation(SelectSequence);

    if (!Data)
        return;

    if (!m_ReverseCheckBox->IsCheck())
        Data->SetReverse(false);
}

void CSpriteWindow::ChangePlayTime()
{
    CObjectHierarchy* Hierarchy = CEditorManager::GetInst()->GetObjectHierarchy();

    CScene* Scene = CSceneManager::GetInst()->GetScene();

    if (Hierarchy->GetObjectList()->GetSelectIndex() == -1)
        return;

    CGameObject* Obj = Scene->FindObject(Hierarchy->GetObjectList()->GetSelectItem());

    if (Obj->GetRootComponent()->GetTypeID() == typeid(CSpriteComponent).hash_code())
    {
        CSpriteComponent* Sprite = (CSpriteComponent*)Obj->GetRootComponent();
        
        Sprite->GetCurrentAnimation()->SetPlayTime(m_AnimPlayTime->GetValueFloat());
    }
}

void CSpriteWindow::ChangePlayScale()
{
}

void CSpriteWindow::StyleCallback()
{
    MyShowStyleEditor();
}

void CSpriteWindow::MyShowStyleEditor(ImGuiStyle* ref)
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

void CSpriteWindow::ClickObjInEditor()
{
    int a = 3;
}


void CSpriteWindow::PlayAnimation()
{
    if (!m_SpriteObject)
        m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");

    m_AnimInstance->Play();
}

void CSpriteWindow::StopAnimation()
{
    if (!m_SpriteObject)
        m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");

    m_AnimInstance->Stop();
}

void CSpriteWindow::DeleteSequenceButton()
{
    int SelectAnimIndex = m_AnimationList->GetSelectIndex();
    if (SelectAnimIndex == -1)
        return;

    if (m_AnimInstance->IsPlay())
        m_AnimInstance->Stop();

    // �ִϸ��̼� ������ ������ �����

    std::string SequenceName = m_AnimationList->GetItem(SelectAnimIndex);

    m_AnimInstance->DeleteAnimation(SequenceName);

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    Resource->EraseAnimationSequence2D(SequenceName);

    m_AnimationList->DeleteItem(SelectAnimIndex);

    m_AnimationFrameList->Clear();
}

void CSpriteWindow::DeleteFrameButton()
{
    int SelectIndex = m_AnimationFrameList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    int SelectAnimIndex = m_AnimationList->GetSelectIndex();
    if (SelectAnimIndex == -1)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Anim = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectAnimIndex));

    Anim->DeleteFrame(SelectIndex);

    m_AnimationFrameList->DeleteItem(SelectIndex);

    int FrameCount = m_AnimationFrameList->GetItemCount();

    for (int i = SelectIndex; i < FrameCount; ++i)
    {
        std::stringstream ss;

        ss << i;

        m_AnimationFrameList->ModifyItem(i, ss.str());
    }

    // Load�� sqc������ Frame�� ������� ���� ������ sqc������ ���� ����
    SaveSequence();
}


