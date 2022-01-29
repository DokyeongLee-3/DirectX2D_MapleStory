#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"

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
	std::vector<std::string> m_vecInventoryItem;

	CSharedPtr<CImage>	m_InventoryBackground;

	CSharedPtr<CImage>	m_EquipmentTab;
	CSharedPtr<CImage>	m_ConsumeTab;
	CSharedPtr<CImage>	m_EtcTab;
	CSharedPtr<CImage>	m_InstallTab;
	CSharedPtr<CImage>	m_CashTab;
	CSharedPtr<CImage>	m_DecorationTab;
	CSharedPtr<CImage>	m_MesoImage;
	CSharedPtr<CImage>	m_MaplePointImage;
	CSharedPtr<CImage>	m_SideScroll;

	Vector2 m_SlotSize;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CInventory* Clone();
};

