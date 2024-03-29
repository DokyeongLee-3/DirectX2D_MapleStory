#pragma once

#include "IMGUIWidget.h"

class CIMGUIComboBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUITree;

protected:
	CIMGUIComboBox();
	virtual ~CIMGUIComboBox();

protected:
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	std::string	m_PrevViewName;
	bool		m_Select;
	int			m_SelectIndex;
	int			m_PageItemCount;
	bool		m_Sort;
	ImGuiComboFlags	m_Flag;
	std::function<void(int, const char*)>	m_SelectCallback;

public:
	const std::string& GetItem(int Index)
	{
		return m_vecItem[Index];
	}

	const std::string& GetSelectItem()	const
	{
		return m_vecItem[m_SelectIndex];
	}

	int GetSelectIndex()	const
	{
		return m_SelectIndex;
	}

	int GetItemCount()	const
	{
		return (int)m_vecItem.size();
	}

	void SetSelectIndex(int Index)
	{
		m_SelectIndex = Index;
	}

public:
	void AddItem(const std::string& Item)
	{
		m_vecItem.push_back(Item);

		wchar_t	wItem[1024] = {};
		char	ItemUTF8[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Item.c_str(), -1, wItem, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wItem, -1, ItemUTF8, Length, 0, 0);

		m_vecItemUTF8.push_back(ItemUTF8);

		if (m_Sort)
		{
			std::sort(m_vecItem.begin(), m_vecItem.end());
			std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
		}
	}

	void SetPageItemCount(int Count)
	{
		m_PageItemCount = Count;
	}

	void DeleteItem(int Index)
	{
		auto	iter = m_vecItem.begin() + Index;

		m_vecItem.erase(iter);

		auto	iter1 = m_vecItemUTF8.begin() + Index;

		m_vecItemUTF8.erase(iter1);
	}

	void Clear()
	{
		m_vecItem.clear();
		m_vecItemUTF8.clear();
	}

	void Sort(bool bSort)
	{
		m_Sort = bSort;

		if (m_Sort)
		{
			std::sort(m_vecItem.begin(), m_vecItem.end());
			std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
		}
	}

	void AddFlag(ImGuiComboFlags_ Flag)
	{
		m_Flag |= Flag;
	}

public:
	virtual bool Init();
	virtual void Render();

public:
	template <typename T>
	void SetSelectCallback(T* Obj, void(T::* Func)(int, const char*))
	{
		m_SelectCallback = std::bind(Func, Obj, std::placeholders::_1, std::placeholders::_2);
	}
};

