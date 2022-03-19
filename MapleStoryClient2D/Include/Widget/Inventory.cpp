

#include "Inventory.h"
#include "Input.h"
#include "Widget/Image.h"
#include "Input.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"

CInventory::CInventory()    :
    m_SlotSize(30.f, 30.f),
    m_CurrentOpenTab(nullptr)
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
    m_BlankCollider->SetPos(10.f, 360.f);
    m_BlankCollider->SetSize(180.f, 20.f);
    m_BlankCollider->SetMouseCollisionEnable(true);
    m_BlankCollider->SetClickCallback(this, &CInventory::DragWindow);
    m_BlankCollider->SetZOrder(2);

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
    m_Money->SetPos(132.f, 61.f);
    m_Money->SetSize(7.f, 9.f);

    for (int i = 0; i < 10; ++i)
    {
        SAFE_DELETE_ARRAY(vecFileName[i]);
    }

    m_CurrentOpenTab = m_EquipmentTab;

    return true;
}

void CInventory::Update(float DeltaTime)
{
    CWidgetWindow::Update(DeltaTime);
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

void CInventory::DragWindow()
{
    Vector2 MouseMove = CInput::GetInst()->GetMouseMove();

    m_Pos += MouseMove;

    m_BlankCollider->SetClicked(false);
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
}


