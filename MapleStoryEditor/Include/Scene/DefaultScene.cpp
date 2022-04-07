
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

    //SAFE_DELETE(m_LoadingThread);
}

CDefaultScene::~CDefaultScene()
{
}

bool CDefaultScene::Init()
{
    CreateMaterial();

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

CGameObject* CDefaultScene::GetCameraObject() const
{
    return m_CameraObject;
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
    m_Scene->GetResource()->LoadSequence2D("SylphideLancerHitPurple.sqc");

    m_Scene->GetResource()->LoadSequence2D("VoidPressureReadyLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureStartLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureOrb.sqc");
    m_Scene->GetResource()->LoadSequence2D("VoidPressureDestroy.sqc");

    m_Scene->GetResource()->LoadSequence2D("DeathSide.sqc");
    m_Scene->GetResource()->LoadSequence2D("DeathSideHitEffect.sqc");

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
    m_Scene->GetResource()->LoadSequence2D("VerticalLampLight.sqc");
    m_Scene->GetResource()->LoadSequence2D("Library2ndButterfly.sqc");
    m_Scene->GetResource()->LoadSequence2D("Library2ndSmallButterfly.sqc");
    m_Scene->GetResource()->LoadSequence2D("LampBigHelix.sqc");
    m_Scene->GetResource()->LoadSequence2D("LampSmallHelix.sqc");

    m_Scene->GetResource()->LoadSequence2D("LowerClassBookIdleLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("LowerClassBookHitLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("LowerClassBookDieLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("LowerClassBookAttackLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("LowerClassBookAttackOrb.sqc");

    m_Scene->GetResource()->LoadSequence2D("ZakumNormalIdle.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumDestroyIdle.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumLeftHand.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumRightHand.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumLeftHandSmash.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumLeftHandSmash2.sqc");

    m_Scene->GetResource()->LoadSequence2D("ZakumFlame.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumClapLeft.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumClapRight.sqc");
    m_Scene->GetResource()->LoadSequence2D("ZakumClapCircleShockWave.sqc");

    m_Scene->GetResource()->LoadSequence2D("FloatingBill.sqc");
    m_Scene->GetResource()->LoadSequence2D("PlayerDead.sqc");

    m_Scene->GetResource()->LoadSequence2D("NPCAdobisIdle.sqc");

    m_Scene->GetResource()->LoadSequence2D("NPCAmonIdle.sqc");

    return true;
}

bool CDefaultScene::CreateMaterial()
{
    m_Scene->GetResource()->CreateMaterial<CMaterial>("TileMap");
    CMaterial* Material = m_Scene->GetResource()->FindMaterial("TileMap");

    //Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "DiabloTile", TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

    Material->SetShader("TileMapShader");
    Material->SetRenderState("AlphaBlend");


    // 파티클들의 Material 만들어주기
    // Rain Particle Material
    m_Scene->GetResource()->CreateMaterial<CMaterial>("Rain");
    Material = m_Scene->GetResource()->FindMaterial("Rain");

    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Rain", TEXT("Particle/HardRain.png"));

    Material->SetShader("ParticleRenderShader");
    Material->SetRenderState("AlphaBlend");

    // Dust Particle Material
    m_Scene->GetResource()->CreateMaterial<CMaterial>("Dust");
    Material = m_Scene->GetResource()->FindMaterial("Dust");

    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Dust", TEXT("Particle/smokeparticle.png"));

    Material->SetShader("ParticleRenderShader");
    Material->SetRenderState("AlphaBlend");

    // Smoke Particle Material
    m_Scene->GetResource()->CreateMaterial<CMaterial>("Smoke");
    Material = m_Scene->GetResource()->FindMaterial("Smoke");

    Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "Smoke", TEXT("Particle/smokeparticle.png"));

    Material->SetShader("ParticleRenderShader");
    Material->SetRenderState("AlphaBlend");

    // Rain Particle Resource 생성
    m_Scene->GetResource()->CreateParticle("Rain");
    CParticle* Particle = m_Scene->GetResource()->FindParticle("Rain");

    Material = m_Scene->GetResource()->FindMaterial("Rain");

    Particle->SetMaterial(Material);
    Particle->SetSpawnCountMax(2000);
    Particle->SetLifeTimeMin(4.f);
    Particle->SetLifeTimeMax(7.f);
    Particle->SetScaleMin(Vector3(61.f, 105.f, 1.f));
    Particle->SetScaleMax(Vector3(61.f, 105.f, 1.f));
    Particle->SetSpeedMin(0.f);
    Particle->SetSpeedMax(0.f);
    Particle->SetMoveDir(Vector3(0.f, -1.f, 0.f));
    Particle->SetStartMin(Vector3(-900.f, -30.f, 0.f));
    Particle->SetStartMax(Vector3(900.f, 30.f, 0.f));
    Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 0.6f));
    Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.8f));
    Particle->SetGravityFactor(15.f);
    //Particle->SetMoveAngle(Vector3(0.f, 0.f, 30.f));
    Particle->SetGravity(true);
    Particle->SetMove(true);

    // Dust Particle Resource 생성
    m_Scene->GetResource()->CreateParticle("Dust");
    Particle = m_Scene->GetResource()->FindParticle("Dust");

    Material = m_Scene->GetResource()->FindMaterial("Dust");

    Particle->SetMaterial(Material);
    Particle->SetSpawnCountMax(1000);
    Particle->SetLifeTimeMin(3.f);
    Particle->SetLifeTimeMax(3.5f);
    Particle->SetScaleMin(Vector3(128.f, 128.f, 1.f));
    Particle->SetScaleMax(Vector3(128.f, 128.f, 1.f));
    Particle->SetSpeedMin(30.f);
    Particle->SetSpeedMax(50.f);
    Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
    Particle->SetStartMin(Vector3(30.f, -10.f, 0.f));
    Particle->SetStartMax(Vector3(-30.f, 10.f, 0.f));
    Particle->SetColorMin(Vector4(0.6f, 0.26f, 0.0196f, 1.f));
    Particle->SetColorMax(Vector4(0.6f, 0.26f, 0.0196f, 0.4f));
    //Particle->SetGravityFactor(1.f);
    Particle->SetMoveAngle(Vector3(0.f, 0.f, 120.f));
    Particle->SetGravity(false);
    Particle->SetMove(true);

    // Smoke Particle Resource 생성
    m_Scene->GetResource()->CreateParticle("Smoke");
    Particle = m_Scene->GetResource()->FindParticle("Smoke");

    Material = m_Scene->GetResource()->FindMaterial("Smoke");

    Particle->SetMaterial(Material);
    Particle->SetSpawnCountMax(1200);
    Particle->SetLifeTimeMin(3.f);
    Particle->SetLifeTimeMax(4.f);
    Particle->SetScaleMin(Vector3(128.f, 128.f, 1.f));
    Particle->SetScaleMax(Vector3(128.f, 128.f, 1.f));
    Particle->SetSpeedMin(70.f);
    Particle->SetSpeedMax(80.f);
    Particle->SetMoveDir(Vector3(0.f, 1.f, 0.f));
    Particle->SetStartMin(Vector3(0.f, -10.f, 0.f));
    Particle->SetStartMax(Vector3(0.f, 10.f, 0.f));
    Particle->SetColorMin(Vector4(1.f, 1.f, 1.f, 1.f));
    Particle->SetColorMax(Vector4(1.f, 1.f, 1.f, 0.4f));
    //Particle->SetGravityFactor(1.f);
    Particle->SetMoveAngle(Vector3(0.f, 0.f, 60.f));
    Particle->SetGravity(false);
    Particle->SetMove(true);

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
