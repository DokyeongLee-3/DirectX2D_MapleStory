#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Button.h"
#include "Widget/Number.h"

struct ItemState
{
	std::string Name;
	Item_Category Category;
	int Count;
	int Row;
	int Column;
	CNumber* ItemCountWidget;
	CImage* ItemIcon;
};

class CInventory :
    public CWidgetWindow
{
	friend class CViewport;

	// 좌하단 슬롯 위치는 좌하단이 (0,0)일때 (14, 295), 슬롯 하나당 크기 (31, 31)
protected:
	CInventory();
	CInventory(const CInventory& window);
	virtual ~CInventory();

private:
	std::vector<ItemState*> m_vecInventoryItem;

	CSharedPtr<CImage>	m_InventoryBackground;

	CSharedPtr<CImage>	m_EquipmentTab;
	CSharedPtr<CImage>	m_ConsumeTab;
	CSharedPtr<CImage>	m_EtcTab;
	CSharedPtr<CImage>	m_InstallTab;
	CSharedPtr<CImage>	m_CashTab;
	CSharedPtr<CImage>	m_DecorationTab;
	CSharedPtr<CButton>	m_MesoImage;
	CSharedPtr<CImage>	m_MaplePointImage;
	CSharedPtr<CImage>	m_SideScroll;
	CSharedPtr<CImage>	m_BlankCollider;
	CSharedPtr<CNumber>	m_Money;

	Vector2 m_SlotSize;

	CImage*				m_CurrentOpenTab;
	Item_Category		m_CurrentOpenTabCatogory;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CInventory* Clone();

public:
	void DragWindow();
	void AddMoney(int Money);
	void ClickEquipTab();
	void ClickConsumeTab();
	void ClickInstallTab();
	void ClickCashTab();
	void ClickDecorationTab();
	void ClickEtcTab();
	void AddItem(const TCHAR* FileName, const std::string& Name, Item_Category Category, const Vector2& IconSize, int Count, int Row, int Column);
	void ConsumeItem(const std::string& Name, int ConsumeCount);
};

