#pragma once

#include "IMGUIWidget.h"

class CIMGUIListBox :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUIListBox();
	virtual ~CIMGUIListBox();

protected:
	std::vector<std::string>	m_vecItem;
	std::vector<std::string>	m_vecItemUTF8;
	bool		m_Select;
	int			m_SelectIndex;
	int			m_PageItemCount;
	bool		m_Sort;
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

	bool CheckItem(const char* Item)
	{
		size_t	Size = m_vecItem.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecItem[i] == Item)
				return true;
		}

		return false;
	}

public:
	void AddItem(const std::string& Item)
	{
		m_vecItem.push_back(Item);

		wchar_t	wItem[1024] = {};
		char	ItemUTF8[1024] = {};

		// 윈도우에선 만약 멀티 바이트<->UTF-8 둘 간의 변형하고 싶을때에는
		//	멀티바이트->유니코드(UTF - 16)->UTF-8
		//	UTF - 8을 멀티바이트로 변형할때에는
		//	UTF - 8->유니코드(UTF - 16)->멀티바이트
		// 이렇게 중간에 UTF-16을 거쳐서 변형해야한다
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

	void SetSelectIndex(int Index)
	{
		m_SelectIndex = Index;	
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

	void ModifyItem(int Index, const std::string& Modify)
	{
		m_vecItem[Index] = Modify;

		wchar_t	wItem[1024] = {};
		char	ItemUTF8[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Modify.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Modify.c_str(), -1, wItem, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wItem, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wItem, -1, ItemUTF8, Length, 0, 0);

		m_vecItemUTF8[Index] = ItemUTF8;

		if (m_Sort)
		{
			std::sort(m_vecItem.begin(), m_vecItem.end());
			std::sort(m_vecItemUTF8.begin(), m_vecItemUTF8.end());
		}
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

