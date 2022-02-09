

#include "Inventory.h"
#include "Input.h"
#include "Widget/Image.h"
#include "Input.h"

CInventory::CInventory()    :
    m_SlotSize(30.f, 30.f)
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
    m_EquipmentTab->SetTexture("EquipmentTab", TEXT("UI/Inventory/EquipmentTabEnable.png"));
    m_EquipmentTab->SetPos(10.f, 335.f);
    m_EquipmentTab->SetSize(29.f, 19.f);
    m_EquipmentTab->SetMouseCollisionEnable(true);

    m_ConsumeTab = CreateWidget<CImage>("ConsumeTab");
    m_ConsumeTab->SetTexture("ConsumeTab", TEXT("UI/Inventory/ConsumeTabDisable.png"));
    m_ConsumeTab->SetPos(41.f, 335.f);
    m_ConsumeTab->SetSize(29.f, 17.f);
    m_ConsumeTab->SetMouseCollisionEnable(true);

    m_EtcTab = CreateWidget<CImage>("EtcTab");
    m_EtcTab->SetTexture("EtcTab", TEXT("UI/Inventory/EtcTabDisable.png"));
    m_EtcTab->SetPos(72.f, 335.f);
    m_EtcTab->SetSize(29.f, 17.f);
    m_EtcTab->SetMouseCollisionEnable(true);

    m_InstallTab = CreateWidget<CImage>("InstallTab");
    m_InstallTab->SetTexture("InstallTab", TEXT("UI/Inventory/InstallTabDisable.png"));
    m_InstallTab->SetPos(103.f, 335.f);
    m_InstallTab->SetSize(29.f, 17.f);
    m_InstallTab->SetMouseCollisionEnable(true);

    m_CashTab = CreateWidget<CImage>("CashTab");
    m_CashTab->SetTexture("CashTab", TEXT("UI/Inventory/CashTabDisable.png"));
    m_CashTab->SetPos(134.f, 335.f);
    m_CashTab->SetSize(29.f, 17.f);
    m_CashTab->SetMouseCollisionEnable(true);

    m_BlankCollider = CreateWidget<CImage>("BlankCollider");
    m_BlankCollider->SetTexture("BlankCollider", TEXT("UI/BlankCollider.png"));
    m_BlankCollider->SetPos(10.f, 360.f);
    m_BlankCollider->SetSize(180.f, 20.f);
    m_BlankCollider->SetMouseCollisionEnable(true);
    m_BlankCollider->SetClickCallback(this, &CInventory::DragWindow);
    m_BlankCollider->SetZOrder(2);

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


