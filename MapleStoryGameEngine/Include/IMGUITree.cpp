#include "IMGUITree.h"


CIMGUITree::CIMGUITree() :
	m_Root(nullptr)
{
}

CIMGUITree::~CIMGUITree()
{
	size_t Size = m_vecNode.size();

	for (int i = 0; i < Size; ++i)
	{
		size_t WidgetSize = m_vecNode[i]->vecWidget.size();

		for (int j = 0; j < WidgetSize; ++j)
		{
			SAFE_DELETE(m_vecNode[i]->vecWidget[j]);
		}

		SAFE_DELETE(m_vecNode[i]);
	}
}

bool CIMGUITree::Init()
{
	return true;
}

void CIMGUITree::Render()
{

	ImGui::PushItemWidth(m_Size.x);

	if (ImGui::CollapsingHeader(m_Root->NameUTF8.c_str()))
	{
		size_t Size = m_Root->vecChild.size();

		for (int i = 0; i < Size; ++i)
		{
			RenderChild(m_Root->vecChild[i]);
		}
	}

}

void CIMGUITree::RenderChild(Node* MyNode)
{
	if (ImGui::TreeNode(MyNode->NameUTF8.c_str()))
	{
		if (MyNode->Callback)
			MyNode->Callback();

		size_t Size = MyNode->vecChild.size();

		for (size_t i = 0; i < Size; ++i)
		{
			RenderChild(MyNode->vecChild[i]);
		}

		Size = MyNode->vecWidget.size();

		for (size_t i = 0; i < Size; ++i)
		{
			MyNode->vecWidget[i]->Render();
		}

		ImGui::TreePop();
		ImGui::Separator();
	}
}
