
#include "DefaultScene.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Input.h"
#include "Device.h"
#include "../EditorManager.h"
#include "../Window/SpriteWindow.h"
#include "../Window/ObjectHierarchy.h"
#include "../Object/SpriteEditObject.h"
#include "Component/SpriteComponent.h"
#include "IMGUIListBox.h"
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

    m_CameraComponent = m_CameraObject->CreateComponent<CCameraComponent>("CameraComponent");

    m_CameraObject->SetWorldPos(0.f, 0.f, 0.f);
    m_CameraObject->SetWorldScale(1.f, 1.f, 1.f);
    m_CameraObject->SetPivot(0.5f, 0.5f, 0.f);
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


    return true;
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

    m_Scene->GetResource()->LoadSequence2D("DoubleHelixBlinkTree.sqc");
    m_Scene->GetResource()->LoadSequence2D("SingleHelixBlinkTree.sqc");

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

            if (m_CameraObject->GetWorldPos().x + RS.Width >= Width)
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

        if (m_CameraObject->GetWorldPos().x + RS.Width >= Width)
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
        if (m_CameraObject->GetWorldPos().x <= 0.f)
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

        if (m_CameraObject->GetWorldPos().x <= 0.f)
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

            if (m_CameraObject->GetWorldPos().y + RS.Height >= Height)
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

        if (m_CameraObject->GetWorldPos().y + RS.Height >= Height)
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
        if (m_CameraObject->GetWorldPos().y <= 0)
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

        if (m_CameraObject->GetWorldPos().y <= 0)
        {
            m_CameraObject->SetWorldPos(m_CameraObject->GetWorldPos().x, 0.f, 0.f);
            return;
        }

        m_CameraObject->AddRelativePos(0.f, -300.f * DeltaTime, 0.f);
    }
}

void CDefaultScene::AddObjectList(const char* ObjName)
{
    CEditorManager::GetInst()->GetObjectHierarchy()->GetObjectList()->AddItem(ObjName);
}

void CDefaultScene::AddComponentList(const char* ComponentName)
{
    CEditorManager::GetInst()->GetObjectHierarchy()->GetComponentList()->AddItem(ComponentName);
}
