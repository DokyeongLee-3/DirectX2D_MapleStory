#pragma once

#include "Widget/WidgetWindow.h"
#include "Widget/Image.h"
#include "Widget/Button.h"
#include "Widget/Number.h"

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
	int					m_LastAddRow;
	int					m_LastAddColumn;
	int					m_RowLimit;
	int					m_ColumnLimit;

public:
	virtual void Start();
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual void Render();
	virtual CInventory* Clone();

public:
	int GetLastAddRow()	const
	{
		return m_LastAddRow;
	}

	int GetLastAddColumn()	const
	{
		return m_LastAddColumn;
	}

	int GetRowLimit()	const
	{
		return m_RowLimit;
	}

	int GetColumnLimit()	const
	{
		return m_ColumnLimit;
	}

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
	ItemState* FindItemState(const std::string& Name);
	void ReturnScrollUse();
	void ToolTipWindowSceneChange(class CScene* Scene, class CViewport* Viewport);

public:
	int GetCategoryItemCount(Item_Category Category)
	{
		// 지금 추가되는 아이템과 같은 카테고리에 속한 인벤토리에 아이템 개수
		int CategoryCount = 0;
		size_t InvenCount = m_vecInventoryItem.size();

		for (size_t i = 0; i < InvenCount; ++i)
		{
			if (m_vecInventoryItem[i]->Category == Category)
				++CategoryCount;
		}

		return CategoryCount;
	}

public:
	void ShowBroiledEelsToolTip();
	void ShowReturnScrollToolTip();
	void HideBroiledEelsToolTip();
	void HideReturnScrollToolTip();
	void ShowItemOnionToolTip();
	void ShowItemRadishToolTip();
	void HideItemOnionToolTip();
	void HideItemRadishToolTip();
	void TurnOffAllToolTip();
};

