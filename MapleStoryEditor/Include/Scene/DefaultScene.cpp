
#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Input.h"
#include "Device.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"
#include "../Window/ObjectHierarchy.h"
#include "../Window/DetailWindow.h"
#include "IMGUITextInput.h"
#include "Component/SpriteComponent.h"
#include "IMGUIListBox.h"
#include "../Object/SpriteEditObject.h"
#include "../Object/Stage.h"

CDefaultScene::CDefaultScene()  :
    m_CameraObject(nullptr),
    m_CameraComponent(nullptr),
    m_StageObject(nullptr)
{
    SetTypeID<CDefaultScene>();
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
    m_CameraObject = m_Scene->CreateGameObject<CGameObject>("EditorCamera");
    m_CameraObject->SetScene(m_Scene);

    m_CameraComponent = m_CameraObject->CreateComponent<CCameraComponent>("CameraComponent");

    m_CameraObject->SetWorldPos(0.f, 0.f, 0.f);
    m_CameraObject->SetWorldScale(0.f, 0.f, 0.f);
    m_CameraObject->SetPivot(0.f, 0.f, 0.f);
    m_CameraObject->SetRootComponent(m_CameraComponent);

    m_Scene->GetCameraManager()->SetCurrentCamera(m_CameraComponent);

    if (!LoadAnimationSequence2D())
        return false;

    CInput::GetInst()->CreateKey("CameraDown", VK_DOWN);
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("CameraDown", KeyState_Push, this, &CDefaultScene::CameraDown);

    CInput::GetInst()->CreateKey("CameraUp", VK_UP);
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("CameraUp", KeyState_Push, this, &CDefaultScene::CameraUp);

    CInput::GetInst()->CreateKey("CameraRight", VK_RIGHT);
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("CameraRight", KeyState_Push, this, &CDefaultScene::CameraRight);

    CInput::GetInst()->CreateKey("CameraLeft", VK_LEFT);
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("CameraLeft", KeyState_Push, this, &CDefaultScene::CameraLeft);


    CInput::GetInst()->CreateKey("SelectObjectDown", 'S');
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("SelectObjectDown", KeyState_Down, this, &CDefaultScene::SelectObjectDown);

    CInput::GetInst()->CreateKey("SelectObjectUp", 'W');
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("SelectObjectUp", KeyState_Down, this, &CDefaultScene::SelectObjectUp);

    CInput::GetInst()->CreateKey("SelectObjectRight", 'D');
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("SelectObjectRight", KeyState_Down, this, &CDefaultScene::SelectObjectRight);

    CInput::GetInst()->CreateKey("SelectObjectLeft", 'A');
    CInput::GetInst()->SetKeyCallback<CDefaultScene>("SelectObjectLeft", KeyState_Down, this, &CDefaultScene::SelectObjectLeft);

    return true;
}

void CDefaultScene::SetStageObject(CStage* Stage)
{
    m_StageObject = Stage;
}

bool CDefaultScene::LoadAnimationSequence2D()
{

    m_Scene->GetResource()->LoadSequence2D("PlayerIdleLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerWalkLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerHealLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerRope.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerJumpLeft.sqc");

    m_Scene->GetResource()->LoadSequence2D("PlayerOrb.sqc");

    m_Scene->GetResource()->LoadSequence2D("SylphideLancerBodyEffectLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("SylphideLancerArrowLeft.sqc");


    m_Scene->GetResource()->LoadSequence2D("VoidPressureReadyLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureStartLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureOrb.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureDestroy.sqc");

    m_Scene->GetResource()->LoadSequence2D("RadishIdleLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("RadishWalkLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("RadishHitLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("RadishDieLeft.sqc");

    m_Scene->GetResource()->LoadSequence2D("OnionIdleLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("OnionWalkLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("OnionHitLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("OnionDieLeft.sqc");

    m_Scene->GetResource()->LoadSequence2D("SingleHelixBlinkTree.sqc");
    m_Scene->GetResource()->LoadSequence2D("DoubleHelixBlinkTree.sqc");
    m_Scene->GetResource()->LoadSequence2D("BlinkTree.sqc");

    m_Scene->GetResource()->LoadSequence2D("Portal.sqc");
    m_Scene->GetResource()->LoadSequence2D("Butterfly.sqc");
    m_Scene->GetResource()->LoadSequence2D("LampLight.sqc");

    return true;
}

void CDefaultScene::CameraRight(float DeltaTime)
{
    Resolution RS = CDevice::GetInst()->GetResolution();

    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        CSpriteEditObject* SpriteObj = CEditorManager::GetInst()->GetSpriteWindow()->GetSpriteObject();

        if (SpriteObj)
        {
            float Width = (float)(SpriteObj->GetSpriteComponent()->GetMaterial()->GetTextureWidth());

            if (m_CameraObject->GetWorldPos().x + 300.f * DeltaTime + RS.Width >= Width)
            {
                m_CameraObject->SetWorldPos(Width - RS.Width, m_CameraObject->GetWorldPos().y, 0.f);
                return;
            }  

            m_CameraObject->AddRelativePos(300.f * DeltaTime, 0.f, 0.f);
        }
    }

    else if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
    {
        if (!m_StageObject)
            return;

        float Width = (float)(m_StageObject->GetSpriteComponent()->GetMaterial()->GetTextureWidth());

        CDefaultScene* Me = this;

        if (m_CameraObject->GetWorldPos().x + 300.f * DeltaTime + RS.Width >= Width)
        {
            m_CameraObject->SetWorldPos(Width - RS.Width, m_CameraObject->GetWorldPos().y, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(300.f * DeltaTime, 0.f, 0.f);
    }
}

void CDefaultScene::CameraLeft(float DeltaTime)
{

    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        if (m_CameraObject->GetWorldPos().x -300.f * DeltaTime <= 0.f)
        {
            m_CameraObject->SetWorldPos(0.f, m_CameraObject->GetWorldPos().y, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(-300.f * DeltaTime, 0.f, 0.f);
    }

    else if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
    {
        if (!m_StageObject)
            return;

        if (m_CameraObject->GetWorldPos().x - 300.f * DeltaTime <= 0.f)
        {
            m_CameraObject->SetWorldPos(0.f, m_CameraObject->GetWorldPos().y, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(-300.f * DeltaTime, 0.f, 0.f);
    }

}

void CDefaultScene::CameraUp(float DeltaTime)
{
    Resolution RS = CDevice::GetInst()->GetResolution();

    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        CSpriteEditObject* SpriteObj = CEditorManager::GetInst()->GetSpriteWindow()->GetSpriteObject();

        if (SpriteObj)
        {
            float Height = (float)(SpriteObj->GetSpriteComponent()->GetMaterial()->GetTextureHeight());

            if (m_CameraObject->GetWorldPos().y + 300.f * DeltaTime + RS.Height >= Height)
            {
                m_CameraObject->SetWorldPos(m_CameraObject->GetWorldPos().x, Height - RS.Height, 0.f);
                return;
            }
        }

        m_CameraObject->AddRelativePos(0.f, 300.f * DeltaTime, 0.f);
    }

    else if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
    {
        if (!m_StageObject)
            return;

        float Height = (float)(m_StageObject->GetSpriteComponent()->GetMaterial()->GetTextureHeight());

        if (m_CameraObject->GetWorldPos().y + 300.f * DeltaTime + RS.Height >= Height)
        {
            m_CameraObject->SetWorldPos(m_CameraObject->GetWorldPos().x, Height - RS.Height, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(0.f, 300.f * DeltaTime, 0.f);
    }
}

void CDefaultScene::CameraDown(float DeltaTime)
{
    if (CEditorManager::GetInst()->GetEditMode() == EditMode::Sprite)
    {
        if (m_CameraObject->GetWorldPos().y - 300.f * DeltaTime <= 0)
        {
            m_CameraObject->SetWorldPos(m_CameraObject->GetWorldPos().x, 0.f, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(0.f, -300.f * DeltaTime, 0.f);
    }

    else if (CEditorManager::GetInst()->GetEditMode() == EditMode::Scene)
    {
        if (!m_StageObject)
            return;

        if (m_CameraObject->GetWorldPos().y - 300.f * DeltaTime <= 0)
        {
            m_CameraObject->SetWorldPos(m_CameraObject->GetWorldPos().x, 0.f, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(0.f, -300.f * DeltaTime, 0.f);
    }
}

void CDefaultScene::SelectObjectRight(float DeltaTime)
{
    CIMGUIListBox* ObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList();

    int Idx = ObjListBox->GetSelectIndex();

    if (Idx == -1)
        return;

    std::string SelectObjectName = ObjListBox->GetSelectItem();

    CGameObject* Obj = m_Scene->FindObject(SelectObjectName);

    if (!Obj)
        return;

    Obj->AddWorldPos(1.f, 0.f, 0.f);

    CDetailWindow* Window = CEditorManager::GetInst()->GetDetailWindow();
    Window->GetPosXInput()->SetValueFloat(Obj->GetWorldPos().x);

}

void CDefaultScene::SelectObjectLeft(float DeltaTime)
{
    CIMGUIListBox* ObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList();

    int Idx = ObjListBox->GetSelectIndex();

    if (Idx == -1)
        return;

    std::string SelectObjectName = ObjListBox->GetSelectItem();

    CGameObject* Obj = m_Scene->FindObject(SelectObjectName);

    if (!Obj)
        return;

    Obj->AddWorldPos(-1.f, 0.f, 0.f);

    CDetailWindow* Window = CEditorManager::GetInst()->GetDetailWindow();
    Window->GetPosXInput()->SetValueFloat(Obj->GetWorldPos().x);
}

void CDefaultScene::SelectObjectUp(float DeltaTime)
{
    CIMGUIListBox* ObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList();

    int Idx = ObjListBox->GetSelectIndex();

    if (Idx == -1)
        return;

    std::string SelectObjectName = ObjListBox->GetSelectItem();

    CGameObject* Obj = m_Scene->FindObject(SelectObjectName);

    if (!Obj)
        return;

    Obj->AddWorldPos(0.f, 1.f, 0.f);

    CDetailWindow* Window = CEditorManager::GetInst()->GetDetailWindow();
    Window->GetPosYInput()->SetValueFloat(Obj->GetWorldPos().y);
}

void CDefaultScene::SelectObjectDown(float DeltaTime)
{
    CIMGUIListBox* ObjListBox = CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList();

    int Idx = ObjListBox->GetSelectIndex();

    if (Idx == -1)
        return;

    std::string SelectObjectName = ObjListBox->GetSelectItem();

    CGameObject* Obj = m_Scene->FindObject(SelectObjectName);

    if (!Obj)
        return;

    Obj->AddWorldPos(0.f, -1.f, 0.f);

    CDetailWindow* Window = CEditorManager::GetInst()->GetDetailWindow();
    Window->GetPosYInput()->SetValueFloat(Obj->GetWorldPos().y);
}

void CDefaultScene::AddObjectList(const char* ObjName)
{
    CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->AddItem(ObjName);
}

void CDefaultScene::AddComponentList(const char* ComponentName)
{
    CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentList()->AddItem(ComponentName);
}
