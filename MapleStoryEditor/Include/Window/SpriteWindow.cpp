
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
#include "Engine.h"
#include "PathManager.h"
#include "../EditorManager.h"
#include "Scene/SceneManager.h"
#include "Component/SpriteComponent.h"
#include "Resource/Animation/AnimationSequence2D.h"
#include "Animation/AnimationSequence2DInstance.h"
#include "Animation/AnimationSequence2DData.h"
#include "../Object/SpriteEditObject.h"
#include "../Object/Pivot.h"
#include "../Object/DragObject.h"
#include "ObjectHierarchy.h"
#include "IMGUIRadioButton.h"
#include "IMGUITree.h"
#include "../Scene/DefaultScene.h"

#include <sstream>

CSpriteWindow::CSpriteWindow() :
    m_SpriteObject(nullptr),
    m_SpriteFrame(nullptr),
    m_AnimInstance(nullptr),
    m_FrameStartPosX(nullptr),
    m_FrameEndPosX(nullptr),
    m_FrameStartPosY(nullptr),
    m_FrameEndPosY(nullptr),
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

float CSpriteWindow::GetAnimPlayTime() const
{
    return m_AnimPlayTime->GetValueFloat();
}

void CSpriteWindow::SetPlayTime(float PlayTime)
{
    m_AnimPlayTime->SetValueFloat(PlayTime);
}

void CSpriteWindow::SetSpriteObject(CSpriteEditObject* Obj)
{
    m_SpriteObject = Obj;
}

void CSpriteWindow::SetSpriteFrameTexture(const std::string& Name, const TCHAR* FileName)
{
    m_SpriteFrame->SetTexture(Name, FileName);
}

void CSpriteWindow::SetSpriteFrameImageStart(const Vector2 Start)
{
    m_SpriteFrame->SetImageStart(Start.x, Start.y);
}

void CSpriteWindow::SetSpriteFrameImageEnd(const Vector2 End)
{
    m_SpriteFrame->SetImageEnd(End.x, End.y);
}

bool CSpriteWindow::Init()
{
    CIMGUIWindow::Init();

    m_SceneEditRadioButton = AddWidget<CIMGUIRadioButton>("SceneEditButton", 50.f, 40.f);
    CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");
    m_SpriteEditRadioButton = AddWidget<CIMGUIRadioButton>("SpriteEditButton", 50.f, 40.f);
    Line = AddWidget<CIMGUISameLine>("Line");
    m_MapEditRadioButton = AddWidget<CIMGUIRadioButton>("MapEditButton", 50.f, 40.f);

    m_SceneEditRadioButton->AddText<CSpriteWindow>("Scene Edit Mode", this, &CSpriteWindow::ObjectArrangeButton);
    m_SceneEditRadioButton->AddActive(true);

    m_SpriteEditRadioButton->AddText<CSpriteWindow>("Sprite Edit Mode", this, &CSpriteWindow::SpriteEditButton);
    m_SpriteEditRadioButton->AddActive(true);

    m_MapEditRadioButton->AddText<CSpriteWindow>("Map Edit Mode", this, &CSpriteWindow::MapEditButton);
    m_MapEditRadioButton->AddActive(true);

    // 일단 맨처음은 Scene Edit Mode로 설정해놨으니 처음 초기화에선 Radio Button중 Scene Edit Mode 선택했을때 콜백을 호출해준다
    ObjectArrangeButton();

    CIMGUILabel* Label = AddWidget<CIMGUILabel>("", 600.f, 20.f);
    //Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);
    Label->SetColor(0, 0, 0, 0);

    CIMGUIButton* Button = AddWidget<CIMGUIButton>("Load a Texture");

    Button->SetSize(120.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadTextureButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_LoadFileName = AddWidget<CIMGUITextInput>("LoadFileName");
    m_LoadFileName->SetHideName(true);
    m_LoadFileName->SetText("");
    m_LoadFileName->SetSize(200.f, 20.f);
    m_LoadFileName->ReadOnly(true);

    Label = AddWidget<CIMGUILabel>("Animation Sequence", 170.f, 30.f);

    //Label->SetColorFloat(0.28f, 0.28f, 0.28f, 0.8f);
    Label->SetColor(71, 71, 71, 204);
    Label->SetAlign(0.5f, 0.f);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(185.f);

    Label = AddWidget<CIMGUILabel>("Frames", 100.f, 30.f);

    //Label->SetColorFloat(0.28f, 0.28f, 0.28f, 0.8f);
    Label->SetColor(71, 71, 71, 204);
    Label->SetAlign(0.5f, 0.f);

    m_AnimationList = AddWidget<CIMGUIListBox>("AnimationList", 170.f, 300.f);
    m_AnimationList->SetHideName(true);
    m_AnimationList->SetPageItemCount(6);
    m_AnimationList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimation);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_AnimationFrameList = AddWidget<CIMGUIListBox>("AnimationFrameList", 100.f, 220.f);
    m_AnimationFrameList->SetHideName(true);
    m_AnimationFrameList->SetPageItemCount(6);
    m_AnimationFrameList->SetSelectCallback<CSpriteWindow>(this, &CSpriteWindow::SelectAnimationFrame);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_SpriteFrame = AddWidget<CIMGUIImage>("SpriteFrame", 115.f, 115.f);

    m_AnimName = AddWidget<CIMGUITextInput>("AnimNameInput", 130.f, 30.f);
    m_AnimName->SetHideName(true);
    m_AnimName->SetHintText("Enter Sequence Name");

    Button = AddWidget<CIMGUIButton>("Add", 35.f, 30.f);
    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Erase", 35.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteSequenceButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Load", 35.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::LoadSequence);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Save", 35.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::SaveSequence);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetOffsetX(190.f);

    Button = AddWidget<CIMGUIButton>("Add##", 35.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::AddAnimationFrameButton);

    Line = AddWidget<CIMGUISameLine>("Line");

    Button = AddWidget<CIMGUIButton>("Erase##", 35.f, 30.f);

    Button->SetClickCallback<CSpriteWindow>(this, &CSpriteWindow::DeleteFrameButton);




    m_FrameStartPosX = AddWidget<CIMGUITextInput>("FrameStartPosX", 100.f, 100.f);
    m_FrameStartPosX->SetFloat(0.f);
    m_FrameStartPosX->SetTextType(ImGuiText_Type::Float);
    m_FrameStartPosX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataStartX);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_FrameStartPosY = AddWidget<CIMGUITextInput>("FrameStartPosY", 100.f, 100.f);
    m_FrameStartPosY->SetFloat(0.f);
    m_FrameStartPosY->SetTextType(ImGuiText_Type::Float);
    m_FrameStartPosY->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataStartY);
    //FrameStartPos->ReadOnly(true);

    m_FrameEndPosX = AddWidget<CIMGUITextInput>("FrameEndPosX", 100.f, 100.f);
    m_FrameEndPosX->SetFloat(100);
    m_FrameEndPosX->SetTextType(ImGuiText_Type::Float);
    m_FrameEndPosX->SetCallback<CSpriteWindow>(this, &CSpriteWindow::AdjustFrameDataEndX);

    Line = AddWidget<CIMGUISameLine>("Line");
    Line->SetSpacing(16.f);

    m_FrameEndPosY = AddWidget<CIMGUITextInput>("FrameEndPosY", 100.f, 100.f);
    m_FrameEndPosY->SetFloat(100);
    m_FrameEndPosY->SetTextType(ImGuiText_Type::Float);
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
    //Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);
    Label->SetColor(0, 0, 0, 0);

    Label = AddWidget<CIMGUILabel>("CenterXPos", 50.f, 20.f);
    //Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
    Label->SetColor(0, 0, 150, 0);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_DragPivotXPos = AddWidget<CIMGUIText>("CenterXPos");

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("CenterYPos", 50.f, 20.f);
    //Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
    Label->SetColor(0, 0, 150, 0);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_DragPivotYPos = AddWidget<CIMGUIText>("CenterYPos");

    Line = AddWidget<CIMGUISameLine>("Line");

    Label = AddWidget<CIMGUILabel>("CurrentFrame", 90.f, 20.f);
    //Label->SetColorFloat(0.0f, 0.0f, 150.f, 0.f);
    Label->SetColor(0, 0, 150, 0);

    Line = AddWidget<CIMGUISameLine>("Line");

    m_CurrentFrameText = AddWidget<CIMGUIText>("CurrentFrame");


    //Line = AddWidget<CIMGUISameLine>("Line");

    //m_SpriteFrame = AddWidget<CIMGUIImage>("SpriteFrame", 200.f, 200.f);

    //Line = AddWidget<CIMGUISameLine>("Line");


    // SpriteComponent통해서 CreateAnimationInstance를 하면 Instance에 Scene이
    // Set되어 있겠지만 이 경우엔 아니라서 Instance가
    // SpriteCompoent타입인 m_Owner도 없고 속해있는 m_Scene도 없다
    // m_AnimInstance를 이용해서 m_SpriteFrame에 애니메이션 Play버튼 누르면
    // 해당 시퀀스의 애니메이션을 동작하게 만드는데, 이건 셰이더로 그리는게 아니라
    // m_AnimInstance는 상수버퍼도 nullptr이다 
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
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
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

        // 여기 안에서 ImageEnd를 잡아준다(ImageStart는 (0,0) 그대로)
        //m_Sprite->SetTextureFullPath(ConvertFileName, FilePath);

        if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
        {
            m_SpriteObject->GetSpriteComponent()->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel,
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
    m_SpriteEditRadioButton->SetActive(0, true);
    m_SceneEditRadioButton->SetActive(0, false);
    m_MapEditRadioButton->SetActive(0, false);

    CEditorManager::GetInst()->SetEditMode(EditMode::Sprite);

    if (!m_SpriteObject)
    {
        m_SpriteObject = CSceneManager::GetInst()->GetScene()->CreateGameObject<CSpriteEditObject>("SpriteEditObject");
    }
}

void CSpriteWindow::MapEditButton()
{
    m_SpriteEditRadioButton->SetActive(0, false);
    m_SceneEditRadioButton->SetActive(0, false);
    m_MapEditRadioButton->SetActive(0, true);

    CEditorManager::GetInst()->SetEditMode(EditMode::TileMap);

    if (m_SpriteObject)
    {
        CSceneManager::GetInst()->GetScene()->EraseObject(m_SpriteObject);
        m_SpriteObject = nullptr;
    }

    if (m_AnimationList)
        m_AnimationList->Clear();

    if (m_AnimationFrameList)
        m_AnimationFrameList->Clear();
}


void CSpriteWindow::ObjectArrangeButton()
{
    m_SpriteEditRadioButton->SetActive(0, false);
    m_SceneEditRadioButton->SetActive(0, true);
    m_MapEditRadioButton->SetActive(0, false);

    if (m_SpriteObject)
    {
        CSceneManager::GetInst()->GetScene()->EraseObject(m_SpriteObject);
        m_SpriteObject = nullptr;
    }

    if(m_AnimationList)
        m_AnimationList->Clear();

    if(m_AnimationFrameList)
        m_AnimationFrameList->Clear();

    // 바뀌기전이 Sprite모드였다면 크기가 큰 아틀라스 이미지를 보기 위해 움직였던 카메라를 좌하단으로 다시 맞춰준다 
    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        CDefaultScene* DefaultScene = (CDefaultScene*)CSceneManager::GetInst()->GetScene()->GetSceneMode();

        if (DefaultScene)
        {
            CGameObject* Camera = DefaultScene->GetCameraObject();

            if(Camera)
                Camera->SetWorldPos(0.f, 0.f, 0.f);
        }
    }


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

    m_AnimInstance->AddAnimation(Text, Text, true, PlayTime, PlayScale, false);

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
    Vector2 FrameStartPos = Vector2((m_FrameStartPosX->GetValueFloat()), (m_FrameStartPosY->GetValueFloat()));

    Size.x = (m_FrameEndPosX->GetValueFloat() - m_FrameStartPosX->GetValueFloat());
    Size.y = (m_FrameEndPosY->GetValueFloat() - m_FrameStartPosY->GetValueFloat());

    // 이미 m_FrameStartPosX, m_FrameStartPosY, m_FrameEndPosX, m_FrameEndPosY는 드래그위치를 사용하는데 드래그 위치를
    // 잡아줄 때 이미지상의 좌표로 변환해놓았음 
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

    /*Vector2 SpriteFrameSize;
    SpriteFrameSize.x = abs(EndPos.x - StartPos.x);
    SpriteFrameSize.y = abs(EndPos.y - StartPos.y);


    m_SpriteFrame->SetSize(SpriteFrameSize);*/
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

    //m_SpriteFrame->SetSize(Data.Size);

    m_FrameStartPosX->SetFloat(Data.Start.x);
    m_FrameStartPosY->SetFloat(Data.Start.y);
    m_FrameEndPosX->SetFloat(Data.Start.x + Data.Size.x);
    m_FrameEndPosY->SetFloat(Data.Start.y + Data.Size.y);


    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

        if (!DragObj)
            return;

        if (!m_SpriteObject)
            return;

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

        // 빨간 중심점 위치 갱신
        float FrameCenterX = (m_FrameStartPosX->GetValueFloat() + m_FrameEndPosX->GetValueFloat()) / 2.f;
        float FrameCenterY = (m_FrameStartPosY->GetValueFloat() + m_FrameEndPosY->GetValueFloat()) / 2.f;

        Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

        std::stringstream PosX;
        PosX << Center.x;
        std::stringstream PosY;
        PosY << Center.y;

        m_DragPivotXPos->SetText(PosX.str().c_str());
        m_DragPivotYPos->SetText(PosY.str().c_str());
    }
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
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Sequence File\0*.sqc");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    // GetOpenFileName처럼 대화상자에서 선택한 파일의 경로를 얻어올 수 있다는 점에서 같지만
    // GetSaveFileName은 대화상자가 "다른 이름으로 저장" 이라는 타이틀로 열리고 
    // GetOpenFileName은 "열기"라는 타이틀로 열린다
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
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Sequence File\0*.sqc");
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
        // 결국 LoadSequence는 파일에서 정보를 읽어서 시퀀스들을(CAnimationManager에서) 새로 하나 만드는것
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
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Instance File\0*.ins");
    OpenFile.lpstrFile = FilePath;
    OpenFile.nMaxFile = MAX_PATH;
    OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(ANIMATION_PATH)->Path;

    // GetOpenFileName처럼 대화상자에서 선택한 파일의 경로를 얻어올 수 있다는 점에서 같지만
    // GetSaveFileName은 대화상자가 "다른 이름으로 저장" 이라는 타이틀로 열리고 
    // GetOpenFileName은 "열기"라는 타이틀로 열린다
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
    OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0Instance File\0*.ins");
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


    //if (ImGui::ShowStyleSelector("Colors##Selector"))
    //    ref_saved_style = style;
    ImGui::ShowStyleSelector("Colors##Selector");
    ImGui::ShowFontSelector("Fonts##Selector");

}

void CSpriteWindow::DeleteSequenceButton()
{
    int SelectAnimIndex = m_AnimationList->GetSelectIndex();
    if (SelectAnimIndex == -1)
        return;

    if (m_AnimInstance->IsPlay())
        m_AnimInstance->Stop();

    // 애니메이션 시퀀스 실제로 지우기

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

    // Load된 sqc파일의 Frame을 지운것일 수도 있으니 sqc파일을 새로 저장
    SaveSequence();
}


void CSpriteWindow::AdjustFrameDataStartX()
{
    int SelectIndex = m_AnimationList->GetSelectIndex();
    if (SelectIndex == -1)
        return;

    if (!m_SpriteObject)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    float NewFrameStartX = m_FrameStartPosX->GetValueFloat();
    float NewFrameStartY = m_FrameStartPosY->GetValueFloat();
    float NewFrameEndX = m_FrameEndPosX->GetValueFloat();
    float NewFrameEndY = m_FrameEndPosY->GetValueFloat();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);

    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    // 드래그 영역 갱신
    DragObj->SetStartPos(Vector2(m_SpriteObject->GetWorldPos().x + (float)NewFrameStartX, DragObj->GetStartPos().y));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewXPivotPos = (DragObj->GetStartPos().x + DragObj->GetEndPos().x) / 2.f;

    Pivot->SetWorldPos(NewXPivotPos, Pivot->GetWorldPos().y, Pivot->GetWorldPos().z);

    // Center 빨간점 위치 갱신
    float FrameCenterX = (m_FrameStartPosX->GetValueFloat() + m_FrameEndPosX->GetValueFloat()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueFloat() + m_FrameEndPosY->GetValueFloat()) / 2.f;

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

    if (!m_SpriteObject)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    float NewFrameStartX = m_FrameStartPosX->GetValueFloat();
    float NewFrameStartY = m_FrameStartPosY->GetValueFloat();
    float NewFrameEndX = m_FrameEndPosX->GetValueFloat();
    float NewFrameEndY = m_FrameEndPosY->GetValueFloat();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    unsigned int TexHeight = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight();

    // 드래그 영역 갱신
    DragObj->SetStartPos(Vector2(DragObj->GetStartPos().x, m_SpriteObject->GetWorldPos().y + TexHeight - (float)NewFrameStartY));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewYPivotPos = (DragObj->GetStartPos().y + DragObj->GetEndPos().y) / 2.f;

    Pivot->SetWorldPos(Pivot->GetWorldPos().x, NewYPivotPos, Pivot->GetWorldPos().z);

    // Center 빨간점 위치 갱신
    float FrameCenterX = (m_FrameStartPosX->GetValueFloat() + m_FrameEndPosX->GetValueFloat()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueFloat() + m_FrameEndPosY->GetValueFloat()) / 2.f;

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

    if (!m_SpriteObject)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    float NewFrameStartX = m_FrameStartPosX->GetValueFloat();
    float NewFrameStartY = m_FrameStartPosY->GetValueFloat();
    float NewFrameEndX = m_FrameEndPosX->GetValueFloat();
    float NewFrameEndY = m_FrameEndPosY->GetValueFloat();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    // 드래그 영역 갱신
    DragObj->SetEndPos(Vector2(m_SpriteObject->GetWorldPos().x + (float)NewFrameEndX, DragObj->GetEndPos().y));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewXPivotPos = (DragObj->GetStartPos().x + DragObj->GetEndPos().x)/2.f;

    Pivot->SetWorldPos(NewXPivotPos, Pivot->GetWorldPos().y, Pivot->GetWorldPos().z);

    // Center 빨간점 위치 갱신
    float FrameCenterX = (m_FrameStartPosX->GetValueFloat() + m_FrameEndPosX->GetValueFloat()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueFloat() + m_FrameEndPosY->GetValueFloat()) / 2.f;

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

    if (!m_SpriteObject)
        return;

    CSceneResource* Resource = CSceneManager::GetInst()->GetScene()->GetResource();

    CAnimationSequence2D* Sequence = Resource->FindAnimationSequence2D(m_AnimationList->GetItem(SelectIndex));

    float NewFrameStartX = m_FrameStartPosX->GetValueFloat();
    float NewFrameStartY = m_FrameStartPosY->GetValueFloat();
    float NewFrameEndX = m_FrameEndPosX->GetValueFloat();
    float NewFrameEndY = m_FrameEndPosY->GetValueFloat();

    int Index = m_AnimationFrameList->GetSelectIndex();

    if (Index == -1)
        return;

    Vector2 NewStarPos = Vector2((float)NewFrameStartX, (float)NewFrameStartY);
    Vector2 NewSize = Vector2((float)(NewFrameEndX - NewFrameStartX), (float)(NewFrameEndY - NewFrameStartY));

    Sequence->SetFrameData(Index, NewStarPos, NewSize);


    CDragObject* DragObj = CEditorManager::GetInst()->GetDragObj();

    unsigned int TexHeight = m_SpriteObject->GetSpriteComponent()->GetMaterial()->GetTexture()->GetHeight();

    // 드래그 영역 갱신
    DragObj->SetEndPos(Vector2(DragObj->GetEndPos().x, m_SpriteObject->GetWorldPos().y + TexHeight - (float)NewFrameEndY));

    CPivot* Pivot = CEditorManager::GetInst()->GetDragPivot();

    float NewYPivotPos = (DragObj->GetStartPos().y + DragObj->GetEndPos().y) / 2.f;

    Pivot->SetWorldPos(Pivot->GetWorldPos().x, NewYPivotPos, Pivot->GetWorldPos().z);

    // Center 빨간점 위치 갱신
    float FrameCenterX = (m_FrameStartPosX->GetValueFloat() + m_FrameEndPosX->GetValueFloat()) / 2.f;
    float FrameCenterY = (m_FrameStartPosY->GetValueFloat() + m_FrameEndPosY->GetValueFloat()) / 2.f;

    Vector2 Center = Vector2(FrameCenterX, FrameCenterY);

    std::stringstream PosX;
    PosX << Center.x;
    std::stringstream PosY;
    PosY << Center.y;

    m_DragPivotXPos->SetText(PosX.str().c_str());
    m_DragPivotYPos->SetText(PosY.str().c_str());
}




