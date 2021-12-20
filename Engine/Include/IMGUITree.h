#pragma once

#include "IMGUIWidget.h"

struct Node
{
	Node* Parent;
	std::vector<Node*> vecChild;
	std::string Name;
	std::string	NameUTF8;

	// 트리안에 위젯(ex. 리스트박스)들을 갖고 있을 수도 있음
	std::function<void()> Callback;
	std::vector<class CIMGUIWidget*> vecWidget;


	Node() :
		Parent(nullptr)
	{
	}
};

class CIMGUITree :
	public CIMGUIWidget
{
	friend class CIMGUIWindow;

protected:
	CIMGUITree();
	virtual ~CIMGUITree();

protected:
	Node* m_Root;
	std::vector<Node*> m_vecNode;

public:
	Node* GetRoot()	const
	{
		return m_Root;
	}

public:
	Node* AddChildNode(const std::string& ParentName, const std::string& NodeName)
	{
		wchar_t wTmp[1024] = {};
		char	ItemUTF8[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, NodeName.c_str(), -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, NodeName.c_str(), -1, wTmp, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wTmp, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_UTF8, 0, wTmp, -1, ItemUTF8, Length, 0, 0);

		Node* NewNode = new Node;

		if (!m_Root)
		{
			m_Root = NewNode;
			m_Root->Parent = nullptr;
		}

		else
		{
			Node* Parent = FindNode(ParentName);
			NewNode->Parent = Parent;
			Parent->vecChild.push_back(NewNode);
		}

		NewNode->NameUTF8 = ItemUTF8;
		NewNode->Name = NodeName;

		m_vecNode.push_back(NewNode);

		return NewNode;
	}

	Node* FindNode(const std::string& Name)
	{
		if (!m_Root)
			return nullptr;

		size_t Size = m_vecNode.size();

		for (size_t i = 0; i < Size; ++i)
		{
			if (m_vecNode[i]->Name == Name)
				return m_vecNode[i];
		}

		return nullptr;
	}

public:
	template <typename T>
	T* AddNodeWidget(const std::string& NodeName, float Width = 100.f, float Height = 100.f)
	{
		Node* Find = FindNode(NodeName);

		T* Widget = new T;

		Widget->SetName(NodeName);
		Widget->SetSize(Width, Height);

		if (!Widget->Init())
		{
			SAFE_DELETE(Widget);
			return nullptr;
		}

		Find->vecWidget.push_back(Widget);

		return Widget;
	}


public:
	virtual bool Init();
	virtual void Render();
	void RenderChild(Node* MyNode);
};

