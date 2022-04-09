

#include "Inventory.h"
#include "Input.h"
#include "Widget/Image.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "../Scene/WayToZakumScene.h"
#include "../Scene/OnionScene.h"
#include "../Scene/RadishScene.h"
#include "../Scene/Library2ndScene.h"
#include "../Object/Player2D.h"
#include "Scene/SceneManager.h"
#include "Render/RenderManager.h"
#include "ToolTip.h"
#include "Engine.h"

CInventory::CInventory()    :
    m_SlotSize(30.f, 30.f),
    m_CurrentOpenTab(nullptr),
    m_LastAddRow(0),
    m_LastAddColumn(-1),
    m_RowLimit(5),
    m_ColumnLimit(3)
{
}


CInventory::CInventory(const CInventory& window) :
    CWidgetWindow(window)
{
    m_SlotSize = Vector2(30.f, 30.f);

    m_InventoryBackground = window.m_InventoryBackground;

    m_EquipmentTab = window.m_EquipmentTab;
    m_ConsumeTab = window.m_ConsumeTab;
    m_EtcTab = window.m_EtcTab;
    m_InstallTab = window.m_InstallTab;
    m_CashTab = window.m_CashTab;
    m_DecorationTab = window.m_DecorationTab;
    m_MesoImage = window.m_MesoImage;
    m_MaplePointImage = window.m_MaplePointImage;
    m_SideScroll = window.m_SideScroll;
    m_BlankCollider = window.m_BlankCollider;

    auto iter = m_vecInventoryItem.begin();
    auto iterEnd = m_vecInventoryItem.end();

    for (; iter != iterEnd; ++iter)
    {
        m_vecInventoryItem.push_back(*iter);
    }
}

CInventory::~CInventory()
{
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0 ; i < Count; ++i)
    {
        SAFE_DELETE(m_vecInventoryItem[i]);
    }
}

void CInventory::Start()
{
    CWidgetWindow::Start();
}

bool CInventory::Init()
{
    if (!CWidgetWindow::Init())
        return false;

    SetSize(197.f, 380.f);

    m_InventoryBackground = CreateWidget<CImage>("InventoryBackground");
    m_InventoryBackground->SetTexture("InventoryBackground", TEXT("UI/Inventory/InventoryBackground.png"));
    m_InventoryBackground->SetSize(197.f, 380.f);
    m_InventoryBackground->SetMouseCollisionEnable(false);

    m_EquipmentTab = CreateWidget<CImage>("EquipmentTab");
    m_EquipmentTab->SetTexture("EquipmentTabEnable", TEXT("UI/Inventory/EquipmentTabEnable.png"));
    m_EquipmentTab->SetClickCallback<CInventory>(this, &CInventory::ClickEquipTab);
    m_EquipmentTab->SetPos(10.f, 335.f);
    m_EquipmentTab->SetSize(29.f, 19.f);
    m_EquipmentTab->SetMouseCollisionEnable(true);
    m_EquipmentTab->SetZOrder(2);

    m_ConsumeTab = CreateWidget<CImage>("ConsumeTab");
    m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    m_ConsumeTab->SetClickCallback<CInventory>(this, &CInventory::ClickConsumeTab);
    m_ConsumeTab->SetPos(41.f, 335.f);
    m_ConsumeTab->SetSize(29.f, 17.f);
    m_ConsumeTab->SetMouseCollisionEnable(true);
    m_ConsumeTab->SetZOrder(2);

    m_EtcTab = CreateWidget<CImage>("EtcTab");
    m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    m_EtcTab->SetClickCallback<CInventory>(this, &CInventory::ClickEtcTab);
    m_EtcTab->SetPos(72.f, 335.f);
    m_EtcTab->SetSize(29.f, 17.f);
    m_EtcTab->SetMouseCollisionEnable(true);
    m_EtcTab->SetZOrder(2);

    m_InstallTab = CreateWidget<CImage>("InstallTab");
    m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    m_InstallTab->SetClickCallback<CInventory>(this, &CInventory::ClickInstallTab);
    m_InstallTab->SetPos(103.f, 335.f);
    m_InstallTab->SetSize(29.f, 17.f);
    m_InstallTab->SetMouseCollisionEnable(true);
    m_InstallTab->SetZOrder(2);

    m_CashTab = CreateWidget<CImage>("CashTab");
    m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    m_CashTab->SetClickCallback<CInventory>(this, &CInventory::ClickCashTab);
    m_CashTab->SetPos(134.f, 335.f);
    m_CashTab->SetSize(29.f, 17.f);
    m_CashTab->SetMouseCollisionEnable(true);
    m_CashTab->SetZOrder(2);

    m_BlankCollider = CreateWidget<CImage>("BlankCollider");
    m_BlankCollider->SetTexture("BlankCollider", TEXT("UI/BlankCollider.png"));
    m_BlankCollider->SetPos(10.f, 355.f);
    m_BlankCollider->SetSize(180.f, 20.f);
    m_BlankCollider->SetMouseCollisionEnable(true);
    m_BlankCollider->SetClickCallback(this, &CInventory::DragWindow);
    m_BlankCollider->SetZOrder(5);

    m_MesoImage = CreateWidget<CButton>("MesoButton");
    m_MesoImage->SetTexture(Button_State::Normal, "MesoNormal", TEXT("UI/Inventory/meso.png"));
    m_MesoImage->SetTexture(Button_State::MouseOn, "MesoMouseOn", TEXT("UI/Inventory/mesoMouseOn.png"));
    m_MesoImage->SetTexture(Button_State::Click, "MesoClick", TEXT("UI/Inventory/mesopressed.png"));
    m_MesoImage->SetPos(9.f, 58.f);
    m_MesoImage->SetSize(40.f, 16.f);

    m_Money = CreateWidget<CNumber>("Meso");

    std::vector<TCHAR*> vecFileName;

    for (int i = 0; i < 10; ++i)
    {
        TCHAR* FileName = new TCHAR[MAX_PATH];
        memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

        wsprintf(FileName, TEXT("UI/Inventory/StatNumber%d.png"), i);

        vecFileName.push_back(FileName);
    }

    m_Money->SetTexture("Meso", vecFileName);
    m_Money->SetNumber(0);
    m_Money->SetPos(120.f, 60.f);
    m_Money->SetSize(7.f, 9.f);

    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    m_CurrentOpenTab = m_EquipmentTab;
    m_CurrentOpenTabCatogory = Item_Category::Equipment;

    // 장어 구이
    AddItem(TEXT("UI/Inventory/02001527.info.icon.png"), "BroiledEels", Item_Category::Consume, Vector2(31.f, 17.f), 999, 0, 0);

    ItemState* ItState = FindItemState("BroiledEels");

    ItState->ItemIcon->SetHoverCallback<CInventory>(this, &CInventory::ShowBroiledEelsToolTip);
    ItState->ItemIcon->SetHoverEndCallback<CInventory>(this, &CInventory::HideBroiledEelsToolTip);

    // 귀환 주문서
    AddItem(TEXT("UI/Inventory/02030000.info.icon.png"), "ReturnScroll", Item_Category::Consume, Vector2(32.f, 30.f), 100, 0, 1);

    ItState = FindItemState("ReturnScroll");

    ItState->ItemIcon->SetHoverCallback<CInventory>(this, &CInventory::ShowReturnScrollToolTip);
    ItState->ItemIcon->SetHoverEndCallback<CInventory>(this, &CInventory::HideReturnScrollToolTip);

    CImage* Icon = ItState->ItemIcon;
    Icon->SetDoubleClickCallback<CInventory>(this, &CInventory::ReturnScrollUse);

    return true;
}

void CInventory::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0; i < Count; ++i)
    {
        int ItemCount = m_vecInventoryItem[i]->Count;

        m_vecInventoryItem[i]->ItemCountWidget->SetNumber(ItemCount);

        if (m_CurrentOpenTabCatogory != m_vecInventoryItem[i]->Category)
        {
            m_vecInventoryItem[i]->ItemIcon->Enable(false);
            m_vecInventoryItem[i]->ItemCountWidget->Enable(false);
        }

        else
        {
            m_vecInventoryItem[i]->ItemIcon->Enable(true);
            m_vecInventoryItem[i]->ItemCountWidget->Enable(true);
        }
    }
}

void CInventory::PostUpdate(float DeltaTime)
{
    CWidgetWindow::PostUpdate(DeltaTime);

    Vector2 MousePos = CInput::GetInst()->GetMousePos();

    // 슬롯과 슬롯의 가로 간격은 10, 세로 간격은 13
    float OffsetX = 12.f;
    float OffsetY = 13.f;

    if (MousePos.x > m_Pos.x + 14.f && MousePos.x < m_Pos.x + 171.f && MousePos.y > m_Pos.y + 85.f && MousePos.y < m_Pos.y + 325.f)
    {
        // 슬롯의 좌상단부터 시작
        Vector2 SlotLeftTop = Vector2(m_Pos.x + 14.f, m_Pos.y + 325.f);
        
        for (int i = 0; i < 6; ++i)
        {
            for (int j = 0; j < 4; ++j)
            {
                Vector2 NewSlotLeftTop = Vector2(SlotLeftTop.x + (m_SlotSize.x + OffsetX) * j, SlotLeftTop.y + ((m_SlotSize.y + OffsetY) * -i));

                if (MousePos.x > NewSlotLeftTop.x && MousePos.x < NewSlotLeftTop.x + m_SlotSize.x
                    && MousePos.y < NewSlotLeftTop.y && MousePos.y > NewSlotLeftTop.y - m_SlotSize.y)
                {
                    // 인벤토리 창에서 현재 마우스가 올라가있는 Column
                    int Column = j;
                    // 인벤토리 창에서 현재 마우스가 올라가있는 Row
                    int Row = i;

                }
            }
        }
    }
}

void CInventory::Render()
{
    CWidgetWindow::Render();

}

CInventory* CInventory::Clone()
{
    return new CInventory(*this);
}

void CInventory::AddItem(const TCHAR* FileName, const std::string& Name, Item_Category Category, const Vector2& IconSize, 
    int Count, int Row, int Column)
{
    ItemState* ITState = FindItemState(Name);

    if (ITState)
        ITState->Count++;

    else
    {
        if (m_LastAddColumn == m_ColumnLimit)
        {
            ++m_LastAddRow;
            m_LastAddColumn = 0;
        }

        else
        {
            ++m_LastAddColumn;
        }

        ITState = new ItemState;

        ITState->Name = Name;
        ITState->Count = Count;
        ITState->Category = Category;
        ITState->Row = Row;
        ITState->Column = Column;

        std::vector<TCHAR*> vecFileName;

        for (int i = 0; i < 10; ++i)
        {
            TCHAR* FileName = new TCHAR[MAX_PATH];
            memset(FileName, 0, sizeof(TCHAR) * MAX_PATH);

            wsprintf(FileName, TEXT("UI/Inventory/ItemNo.%d.png"), i);

            vecFileName.push_back(FileName);
        }

        // 슬롯과 슬롯의 가로 간격은 10, 세로 간격은 13
        float OffsetX = 12.f;
        float OffsetY = 13.f;

        // 슬롯의 좌상단부터 시작
        Vector2 SlotLeftTop = Vector2(14.f, 325.f);
        Vector2 NewSlotLeftTop = Vector2(SlotLeftTop.x + (m_SlotSize.x + OffsetX) * Column, SlotLeftTop.y + ((m_SlotSize.y + OffsetY) * -Row));

        CImage* ItemIcon = CreateWidget<CImage>(Name);
        ItemIcon->SetTexture(Name, FileName);
        ItemIcon->SetPos(NewSlotLeftTop.x + 1.f, NewSlotLeftTop.y - IconSize.y - 4.f);
        ItemIcon->SetSize(IconSize.x, IconSize.y);
        ItemIcon->SetMouseCollisionEnable(true);
        ItemIcon->SetZOrder(2);
        ITState->ItemIcon = ItemIcon;

        CNumber* ItemNumber = CreateWidget<CNumber>("ItemCountNumber");
        ItemNumber->SetTexture("ItemCountNumber", vecFileName);
        ItemNumber->SetNumber(Count);

        int DigitCount = Count / 10;
        int DigitCount2 = 0;

        if (DigitCount >= 10)
            DigitCount2 = 4;
        else if (DigitCount2 >= 1)
            DigitCount2 = 3;
        else
            DigitCount2 = 2;

        ItemNumber->SetPos(NewSlotLeftTop.x + m_SlotSize.x - (DigitCount2 * 4.f), NewSlotLeftTop.y - m_SlotSize.y - 1.f);
        ItemNumber->SetZOrder(5);
        ITState->ItemCountWidget = ItemNumber;

        for (int i = 0; i < 10; ++i)
        {
            SAFE_DELETE_ARRAY(vecFileName[i]);
        }

        vecFileName.clear();

        m_vecInventoryItem.push_back(ITState);
    }
}

void CInventory::ConsumeItem(const std::string& Name, int ConsumeCount)
{
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0; i < Count; ++i)
    {
        if (m_vecInventoryItem[i]->Name == Name)
        {
            m_vecInventoryItem[i]->Count -= ConsumeCount;

            if (m_vecInventoryItem[i]->Count == 0)
            {
                auto iter = m_vecInventoryItem.begin();

                CImage* Image = m_vecInventoryItem[i]->ItemIcon;
                CNumber* Number = m_vecInventoryItem[i]->ItemCountWidget;

                Image->Destroy();
                Number->Destroy();

                std::advance(iter, i);

                SAFE_DELETE(m_vecInventoryItem[i]);

                m_vecInventoryItem.erase(iter);

                return;
            }
        }
    }
}

ItemState* CInventory::FindItemState(const std::string& Name)
{
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0; i < Count; ++i)
    {
        if (m_vecInventoryItem[i]->Name == Name)
            return m_vecInventoryItem[i];
    }

    return nullptr;
}

void CInventory::ReturnScrollUse()
{
    CScene* CurrentScene = m_Viewport->GetScene();

    if (!CurrentScene)
        return;

    CSceneMode* Mode = CurrentScene->GetSceneMode();

    if (!Mode)
        return;

    CPlayer2D* Player = (CPlayer2D*)CurrentScene->GetPlayerObject();

    if (!Player)
        return;

    if(Player->IsOnLope())
    {
        Player->SetOnLope(false);
    }        

    float DeltaTime = CEngine::GetInst()->GetDeltaTime();

    //Player->ReturnIdle(DeltaTime);

    if (Mode->GetTypeID() == typeid(CWayToZakumScene).hash_code())
    {
        //Player->GetScene()->GetResource()->SoundStop("WayToZakumBGM");
        Player->ClearListCollision();
        Player->SetTileCollisionEnable(false);
        CWayToZakumScene* CurrentMode = (CWayToZakumScene*)Mode;
        CRenderManager::GetInst()->SetStartFadeIn(true);
        CSceneManager::GetInst()->SetFadeInEndCallback<CWayToZakumScene>(CurrentMode, &CWayToZakumScene::CreateLobbyScene);
        ConsumeItem("ReturnScroll", 1);
    }

    else if (Mode->GetTypeID() == typeid(COnionScene).hash_code())
    {
        //Player->GetScene()->GetResource()->SoundStop("OnionSceneBGM");
        Player->ClearListCollision();
        Player->SetTileCollisionEnable(false);
        COnionScene* CurrentMode = (COnionScene*)Mode;
        CRenderManager::GetInst()->SetStartFadeIn(true);
        CSceneManager::GetInst()->SetFadeInEndCallback<COnionScene>(CurrentMode, &COnionScene::CreateLobbyScene);
        ConsumeItem("ReturnScroll", 1);
    }

    else if (Mode->GetTypeID() == typeid(CRadishScene).hash_code())
    {
        //Player->GetScene()->GetResource()->SoundStop("OnionSceneBGM");
        Player->ClearListCollision();
        Player->SetTileCollisionEnable(false);
        CRadishScene* CurrentMode = (CRadishScene*)Mode;
        CRenderManager::GetInst()->SetStartFadeIn(true);
        CSceneManager::GetInst()->SetFadeInEndCallback<CRadishScene>(CurrentMode, &CRadishScene::CreateLobbyScene);
        ConsumeItem("ReturnScroll", 1);
    }

    else if (Mode->GetTypeID() == typeid(CLibrary2ndScene).hash_code())
    {
        //Player->GetScene()->GetResource()->SoundStop("FairyAcademyBGM");
        Player->ClearListCollision();
        Player->SetTileCollisionEnable(false);
        CLibrary2ndScene* CurrentMode = (CLibrary2ndScene*)Mode;
        CRenderManager::GetInst()->SetStartFadeIn(true);
        CSceneManager::GetInst()->SetFadeInEndCallback<CLibrary2ndScene>(CurrentMode, &CLibrary2ndScene::CreateLobbyScene);
        ConsumeItem("ReturnScroll", 1);
    }

    else
        return;
}

void CInventory::ToolTipWindowSceneChange(class CScene* Scene, class CViewport* Viewport)
{
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0; i < Count; ++i)
    {
        if (m_vecInventoryItem[i]->HoverToolTipWindow)
        {
            m_vecInventoryItem[i]->HoverToolTipWindow->SetViewport(Viewport);
            m_vecInventoryItem[i]->HoverToolTipWindow->GetViewport()->SetScene(Scene);
        }
    }
}

void CInventory::ShowBroiledEelsToolTip()
{
    ItemState* ItState = FindItemState("BroiledEels");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
    {
        CScene* CurrentScene = CSceneManager::GetInst()->GetScene();
        CViewport* CurrentViewport = CurrentScene->GetViewport();

        ToolTipWindowSceneChange(CurrentScene, CurrentViewport);

        if (!CurrentViewport->FindWidgetWindow<CToolTip>(ToolTipWindow->GetName()))
            CurrentViewport->AddWindow(ToolTipWindow);

        ToolTipWindow->Enable(true);
        ToolTipWindow->SetZOrder(m_ZOrder + 1);
    }
    
    else
    {
        int ZOrder = ItState->ItemIcon->GetZOrder();
        Vector2 IconPos = ItState->ItemIcon->GetWidgetPos();
        Vector2 IconRenderPos = ItState->ItemIcon->GetRenderPos();

        CToolTip* BroiledEelsToolTipWindow = m_Viewport->CreateWidgetWindow<CToolTip>("BroiledEelsToolTipWindow");
        BroiledEelsToolTipWindow->SetOwnerInventory(this);
        //BroiledEelsToolTipWindow->SetPos(IconPos.x + 240.f, IconPos.y);
        BroiledEelsToolTipWindow->SetPos(IconRenderPos.x - 10.f, IconRenderPos.y - 140.f);
        BroiledEelsToolTipWindow->SetZOrder(m_ZOrder + 1);
        BroiledEelsToolTipWindow->GetToolTipItemIcon()->SetTexture("BroiledEelsToolTipIcon", TEXT("UI/ToolTip/02001527.info.iconRaw.ToolTip.png"));
        BroiledEelsToolTipWindow->GetToolTipItemIcon()->SetSize(54.f, 30.f);
        BroiledEelsToolTipWindow->GetToolTipTitle()->SetText(TEXT("장어 구이"));
        BroiledEelsToolTipWindow->GetToolTipTitle()->SetColor(1.f, 1.f, 1.f);
        BroiledEelsToolTipWindow->GetToolTipTitle()->SetPos(150.f, 128.f);
        BroiledEelsToolTipWindow->GetToolTipContents()->SetText(TEXT("잘 양념된 장어 구이이다.\nHP를 약 1000 회복시켜 준다."));
        BroiledEelsToolTipWindow->GetToolTipContents()->SetColor(1.f, 1.f, 1.f);

        BroiledEelsToolTipWindow->GetToolTipBack()->SetZOrder(ZOrder + 1);
        BroiledEelsToolTipWindow->GetToolTipItemIconBack()->SetZOrder(ZOrder + 2);
        BroiledEelsToolTipWindow->GetToolTipItemIcon()->SetZOrder(ZOrder + 3);
        BroiledEelsToolTipWindow->GetToolTipTitle()->SetZOrder(ZOrder + 2);
        BroiledEelsToolTipWindow->GetToolTipContents()->SetZOrder(ZOrder + 2);

        ItState->HoverToolTipWindow = BroiledEelsToolTipWindow;
    }
}

void CInventory::ShowReturnScrollToolTip()
{
    ItemState* ItState = FindItemState("ReturnScroll");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
    {
        CScene* CurrentScene = CSceneManager::GetInst()->GetScene();
        CViewport* CurrentViewport = CurrentScene->GetViewport();

        ToolTipWindowSceneChange(CurrentScene, CurrentViewport);

        if (!CurrentViewport->FindWidgetWindow<CToolTip>(ToolTipWindow->GetName()))
            CurrentViewport->AddWindow(ToolTipWindow);

        ToolTipWindow->Enable(true);
        ToolTipWindow->SetZOrder(m_ZOrder + 1);
    }

    else
    {
        int ZOrder = ItState->ItemIcon->GetZOrder();
        Vector2 IconPos = ItState->ItemIcon->GetWidgetPos();
        Vector2 IconRenderPos = ItState->ItemIcon->GetRenderPos();

        CToolTip* ReturnScrollToolTipWindow = m_Viewport->CreateWidgetWindow<CToolTip>("ReturnScrollToolTipWindow");
        ReturnScrollToolTipWindow->SetOwnerInventory(this);
        //ReturnScrollToolTipWindow->SetPos(IconPos.x + 240.f, IconPos.y + 15.f);
        ReturnScrollToolTipWindow->SetPos(IconRenderPos.x - 10.f, IconRenderPos.y - 140.f);
        ReturnScrollToolTipWindow->SetZOrder(m_ZOrder + 1);
        ReturnScrollToolTipWindow->GetToolTipItemIcon()->SetTexture("ReturnScrollToolTipIcon", TEXT("UI/ToolTip/02030000.info.iconRaw.ToolTip.png"));
        ReturnScrollToolTipWindow->GetToolTipItemIcon()->SetSize(52.f, 46.f); 
        ReturnScrollToolTipWindow->GetToolTipItemIcon()->SetPos(53.f, 70.f);
        ReturnScrollToolTipWindow->GetToolTipTitle()->SetText(TEXT("마을 귀환 주문서"));
        ReturnScrollToolTipWindow->GetToolTipTitle()->SetColor(1.f, 1.f, 1.f);
        ReturnScrollToolTipWindow->GetToolTipTitle()->SetPos(145.f, 128.f);
        ReturnScrollToolTipWindow->GetToolTipContents()->SetText(TEXT("현재 위치에서 가장 가까운 마을로 귀환할 수 있는 주문서이다. 한번 사용하면 사라진다. 모든 마을 잡화 상점에서 구매할 수 있다."));
        ReturnScrollToolTipWindow->GetToolTipContents()->SetColor(1.f, 1.f, 1.f);
        ReturnScrollToolTipWindow->GetToolTipContents()->SetPos(130.f, 35.f);
        ReturnScrollToolTipWindow->GetToolTipContents()->SetSize(150.f, 90.f);

        ReturnScrollToolTipWindow->GetToolTipBack()->SetZOrder(ZOrder + 1);
        ReturnScrollToolTipWindow->GetToolTipItemIconBack()->SetZOrder(ZOrder + 2);
        ReturnScrollToolTipWindow->GetToolTipItemIcon()->SetZOrder(ZOrder + 3);
        ReturnScrollToolTipWindow->GetToolTipTitle()->SetZOrder(ZOrder + 2);
        ReturnScrollToolTipWindow->GetToolTipContents()->SetZOrder(ZOrder + 2);

        ItState->HoverToolTipWindow = ReturnScrollToolTipWindow;
    }
}

void CInventory::HideBroiledEelsToolTip()
{
    ItemState* ItState = FindItemState("BroiledEels");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
        ToolTipWindow->Enable(false);
}

void CInventory::HideReturnScrollToolTip()
{
    ItemState* ItState = FindItemState("ReturnScroll");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
        ToolTipWindow->Enable(false);
}

void CInventory::ShowItemOnionToolTip()
{
    ItemState* ItState = FindItemState("ItemOnion");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
    {


        CScene* CurrentScene = CSceneManager::GetInst()->GetScene();
        CViewport* CurrentViewport = CurrentScene->GetViewport();

        ToolTipWindowSceneChange(CurrentScene, CurrentViewport);

        if (!CurrentViewport->FindWidgetWindow<CToolTip>(ToolTipWindow->GetName()))
            CurrentViewport->AddWindow(ToolTipWindow);

        //ToolTipWindow->SetPos(IconRenderPos.x - IconSize.x / 2.f, IconRenderPos.y - IconSize.y / 2.f);
        ToolTipWindow->Enable(true);
        ToolTipWindow->SetZOrder(m_ZOrder + 1);
    }

    else
    {
        int ZOrder = ItState->ItemIcon->GetZOrder();
        Vector2 IconPos = ItState->ItemIcon->GetWidgetPos();
        Vector2 IconRenderPos = ItState->ItemIcon->GetRenderPos();

        CToolTip* ItemOnionToolTipWindow = m_Viewport->CreateWidgetWindow<CToolTip>("ItemOnionToolTipWindow");
        ItemOnionToolTipWindow->SetOwnerInventory(this);
        //ItemOnionToolTipWindow->SetPos(IconPos.x + 240.f, IconPos.y);
        ItemOnionToolTipWindow->SetPos(IconRenderPos.x - 10.f, IconRenderPos.y - 140.f);
        ItemOnionToolTipWindow->SetZOrder(m_ZOrder + 1);
        ItemOnionToolTipWindow->GetToolTipItemIcon()->SetTexture("ItemOnionToolTipIcon", TEXT("UI/ToolTip/04000996.info.icon.ToolTip.png"));
        ItemOnionToolTipWindow->GetToolTipItemIcon()->SetSize(50.f, 52.f);
        ItemOnionToolTipWindow->GetToolTipItemIcon()->SetPos(53.f, 70.f);
        ItemOnionToolTipWindow->GetToolTipTitle()->SetText(TEXT("싱싱한 양파"));
        ItemOnionToolTipWindow->GetToolTipTitle()->SetColor(1.f, 1.f, 1.f);
        ItemOnionToolTipWindow->GetToolTipTitle()->SetPos(145.f, 128.f);
        ItemOnionToolTipWindow->GetToolTipContents()->SetText(TEXT("[LV.35] 굉장히 싱싱한 양파.\n야채가 걸어다닐만큼 싱싱해 보인다는 건 바로 이런걸 말하는 게 아닐까."));
        ItemOnionToolTipWindow->GetToolTipContents()->SetColor(1.f, 1.f, 1.f);
        ItemOnionToolTipWindow->GetToolTipContents()->SetPos(130.f, 35.f);
        ItemOnionToolTipWindow->GetToolTipContents()->SetSize(150.f, 90.f);

        ItemOnionToolTipWindow->GetToolTipBack()->SetZOrder(ZOrder + 1);
        ItemOnionToolTipWindow->GetToolTipItemIconBack()->SetZOrder(ZOrder + 2);
        ItemOnionToolTipWindow->GetToolTipItemIcon()->SetZOrder(ZOrder + 3);
        ItemOnionToolTipWindow->GetToolTipTitle()->SetZOrder(ZOrder + 2);
        ItemOnionToolTipWindow->GetToolTipContents()->SetZOrder(ZOrder + 2);

        ItState->HoverToolTipWindow = ItemOnionToolTipWindow;
    }
}

void CInventory::ShowItemRadishToolTip()
{
    ItemState* ItState = FindItemState("ItemRadish");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
    {
        CScene* CurrentScene = CSceneManager::GetInst()->GetScene();
        CViewport* CurrentViewport = CurrentScene->GetViewport();

        ToolTipWindowSceneChange(CurrentScene, CurrentViewport);

        if (!CurrentViewport->FindWidgetWindow<CToolTip>(ToolTipWindow->GetName()))
            CurrentViewport->AddWindow(ToolTipWindow);

        ToolTipWindow->Enable(true);
        ToolTipWindow->SetZOrder(m_ZOrder + 1);
    }

    else
    {
        int ZOrder = ItState->ItemIcon->GetZOrder();
        Vector2 IconPos = ItState->ItemIcon->GetWidgetPos();
        Vector2 IconRenderPos = ItState->ItemIcon->GetRenderPos();

        CToolTip* ItemRadishToolTipWindow = m_Viewport->CreateWidgetWindow<CToolTip>("ItemRadishToolTipWindow");
        ItemRadishToolTipWindow->SetOwnerInventory(this);
        //ItemRadishToolTipWindow->SetPos(IconPos.x + 240.f, IconPos.y);
        ItemRadishToolTipWindow->SetPos(IconRenderPos.x - 10.f, IconRenderPos.y - 140.f);
        ItemRadishToolTipWindow->SetZOrder(m_ZOrder + 1);
        ItemRadishToolTipWindow->GetToolTipItemIcon()->SetTexture("ItemRadishToolTipIcon", TEXT("UI/ToolTip/04000997.info.icon.ToolTip.png"));
        ItemRadishToolTipWindow->GetToolTipItemIcon()->SetSize(52.f, 46.f);
        ItemRadishToolTipWindow->GetToolTipItemIcon()->SetPos(53.f, 70.f);
        ItemRadishToolTipWindow->GetToolTipTitle()->SetText(TEXT("순무의 싹"));
        ItemRadishToolTipWindow->GetToolTipTitle()->SetColor(1.f, 1.f, 1.f);
        ItemRadishToolTipWindow->GetToolTipTitle()->SetPos(145.f, 128.f);
        ItemRadishToolTipWindow->GetToolTipContents()->SetText(TEXT("[LV.35] 굉장히 싱싱한 순무의 싹.\n야채가 걸어다닐만큼 싱싱해 보인다는 건 바로 이런걸 말하는 게 아닐까."));
        ItemRadishToolTipWindow->GetToolTipContents()->SetColor(1.f, 1.f, 1.f);
        ItemRadishToolTipWindow->GetToolTipContents()->SetPos(130.f, 35.f);
        ItemRadishToolTipWindow->GetToolTipContents()->SetSize(155.f, 90.f);
        ItemRadishToolTipWindow->GetToolTipItemIcon()->SetPos(57.f, 71.f);

        ItemRadishToolTipWindow->GetToolTipBack()->SetZOrder(ZOrder + 1);
        ItemRadishToolTipWindow->GetToolTipItemIconBack()->SetZOrder(ZOrder + 2);
        ItemRadishToolTipWindow->GetToolTipItemIcon()->SetZOrder(ZOrder + 3);
        ItemRadishToolTipWindow->GetToolTipTitle()->SetZOrder(ZOrder + 2);
        ItemRadishToolTipWindow->GetToolTipContents()->SetZOrder(ZOrder + 2);

        ItState->HoverToolTipWindow = ItemRadishToolTipWindow;
    }
}

void CInventory::HideItemOnionToolTip()
{
    ItemState* ItState = FindItemState("ItemOnion");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
        ToolTipWindow->Enable(false);
}

void CInventory::HideItemRadishToolTip()
{
    ItemState* ItState = FindItemState("ItemRadish");

    CWidgetWindow* ToolTipWindow = ItState->HoverToolTipWindow;

    if (ToolTipWindow)
        ToolTipWindow->Enable(false);
}

void CInventory::TurnOffAllToolTip()
{
    size_t Count = m_vecInventoryItem.size();

    for (size_t i = 0; i < Count; ++i)
    {
        CWidgetWindow* HoverWindow = m_vecInventoryItem[i]->HoverToolTipWindow;

        if (HoverWindow)
        {
            if (HoverWindow->IsEnable())
                HoverWindow->Enable(false);
        }
    }
}

void CInventory::DragWindow()
{
    int TopMost = m_Viewport->GetTopmostWindowZOrder();

    if (TopMost >= m_ZOrder)
    {
        m_Viewport->DecrementAllWindowZOrder();
        m_ZOrder = TopMost;
    }

    Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

    m_Pos += MouseMove;

    m_BlankCollider->SetClicked(false);

    std::vector<CToolTip*> vecToolTip;

    m_Viewport->FindWidgetWindowByType<CToolTip>(vecToolTip);

    size_t Count = vecToolTip.size();

    for (size_t i = 0; i < Count; ++i)
    {
        Vector2 Pos = vecToolTip[i]->GetWindowPos();

        vecToolTip[i]->SetPos(Pos + MouseMove);
    }

}

void CInventory::AddMoney(int Money)
{
    int CurrentMoney = m_Money->GetNumber();

    CurrentMoney += Money;

    m_Money->SetNumber(CurrentMoney);
}

void CInventory::ClickEquipTab()
{
    m_EquipmentTab->SetTexture("EquipmentTabEnable", TEXT("UI/Inventory/EquipmentTabEnable.png"));
    m_EquipmentTab->SetSize(29.f, 19.f);

    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_CashTab)
    {
        m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_ConsumeTab)
    {
        m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_DecorationTab)
    {
        m_DecorationTab->SetTexture("DecorationTabDisable", TEXT("UI/Inventory/Item.Tab.disabled.5.png"));
    }

    else if (m_CurrentOpenTab == m_EtcTab)
    {
        m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_InstallTab)
    {
        m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    }

    m_CurrentOpenTab = m_EquipmentTab;
    m_CurrentOpenTabCatogory = Item_Category::Equipment;
}

void CInventory::ClickConsumeTab()
{
    m_ConsumeTab->SetTexture("ConsumeTabEnable", TEXT("UI/Inventory/Item.Tab.enabled.1.png"));
    m_ConsumeTab->SetSize(29.f, 19.f);

    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_CashTab)
    {
        m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EquipmentTab)
    {
        m_EquipmentTab->SetTexture("EquipmentTabDisable", TEXT("UI/Inventory/EquipmentTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_DecorationTab)
    {
        m_DecorationTab->SetTexture("DecorationTabDisable", TEXT("UI/Inventory/Item.Tab.disabled.5.png"));
    }

    else if (m_CurrentOpenTab == m_EtcTab)
    {
        m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_InstallTab)
    {
        m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    }

    m_CurrentOpenTab = m_ConsumeTab;
    m_CurrentOpenTabCatogory = Item_Category::Consume;
}

void CInventory::ClickInstallTab()
{
    m_InstallTab->SetTexture("InstallTabEnable", TEXT("UI/Inventory/Item.Tab.enabled.3.png"));
    m_InstallTab->SetSize(29.f, 19.f);

    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_CashTab)
    {
        m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EquipmentTab)
    {
        m_EquipmentTab->SetTexture("EquipmentTabDisable", TEXT("UI/Inventory/EquipmentTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_DecorationTab)
    {
        m_DecorationTab->SetTexture("DecorationTabDisable", TEXT("UI/Inventory/Item.Tab.disabled.5.png"));
    }

    else if (m_CurrentOpenTab == m_EtcTab)
    {
        m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_ConsumeTab)
    {
        m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    }

    m_CurrentOpenTab = m_InstallTab;
    m_CurrentOpenTabCatogory = Item_Category::Install;
}

void CInventory::ClickCashTab()
{
    m_CashTab->SetTexture("CashTabEnable", TEXT("UI/Inventory/Item.Tab.enabled.4.png"));
    m_CashTab->SetSize(29.f, 19.f);

    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_InstallTab)
    {
        m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EquipmentTab)
    {
        m_EquipmentTab->SetTexture("EquipmentTabDisable", TEXT("UI/Inventory/EquipmentTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_DecorationTab)
    {
        m_DecorationTab->SetTexture("DecorationTabDisable", TEXT("UI/Inventory/Item.Tab.disabled.5.png"));
    }

    else if (m_CurrentOpenTab == m_EtcTab)
    {
        m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_ConsumeTab)
    {
        m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    }

    m_CurrentOpenTab = m_CashTab;
    m_CurrentOpenTabCatogory = Item_Category::Cash;
}

void CInventory::ClickDecorationTab()
{
    m_DecorationTab->SetTexture("DecorationTabEnable", TEXT("UI/Inventory/Item.Tab.enabled.5.png"));
    m_DecorationTab->SetSize(29.f, 19.f);

    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_InstallTab)
    {
        m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EquipmentTab)
    {
        m_EquipmentTab->SetTexture("EquipmentTabDisable", TEXT("UI/Inventory/EquipmentTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_CashTab)
    {
        m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EtcTab)
    {
        m_EtcTab->SetTexture("EtcTabDisable", TEXT("UI/Inventory/EtcTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_ConsumeTab)
    {
        m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    }

    m_CurrentOpenTab = m_DecorationTab;
    m_CurrentOpenTabCatogory = Item_Category::Decoration;
}

void CInventory::ClickEtcTab()
{
    m_EtcTab->SetTexture("ETCTabEnable", TEXT("UI/Inventory/Item.Tab.enabled.2.png"));
    m_EtcTab->SetSize(29.f, 19.f);
    
    m_CurrentOpenTab->SetSize(29.f, 17.f);
    m_CurrentOpenTab->SetClicked(false);

    m_Viewport->GetScene()->GetResource()->SoundPlay("TabClick");

    if (m_CurrentOpenTab == m_InstallTab)
    {
        m_InstallTab->SetTexture("InstallTabDisable", TEXT("UI/Inventory/InstallTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_EquipmentTab)
    {
        m_EquipmentTab->SetTexture("EquipmentTabDisable", TEXT("UI/Inventory/EquipmentTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_CashTab)
    {
        m_CashTab->SetTexture("CashTabDisable", TEXT("UI/Inventory/CashTabDisable.png"));
    }

    else if (m_CurrentOpenTab == m_DecorationTab)
    {
        m_DecorationTab->SetTexture("DecorationTabDisable", TEXT("UI/Inventory/Item.Tab.disabled.5.png"));
    }

    else if (m_CurrentOpenTab == m_ConsumeTab)
    {
        m_ConsumeTab->SetTexture("ConsumeTabDisable", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    }

    m_CurrentOpenTab = m_EtcTab;
    m_CurrentOpenTabCatogory = Item_Category::Etc;
}



