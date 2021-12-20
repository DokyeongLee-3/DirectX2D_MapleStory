#include "IMGUIListBox.h"

CIMGUIListBox::CIMGUIListBox() :
    m_Select(false),
    m_SelectIndex(-1),
    m_PageItemCount(3)
{
}

CIMGUIListBox::~CIMGUIListBox()
{
}

bool CIMGUIListBox::Init()
{
    return true;
}

void CIMGUIListBox::Render()
{
    ImVec2  Size = ImVec2(m_Size.x, m_PageItemCount * ImGui::GetTextLineHeightWithSpacing());

    //ImGui::Text(m_Name.c_str());

    if (ImGui::BeginListBox(m_Name.c_str(), Size))
    {
        size_t  ItemCount = m_vecItemUTF8.size();

        for (size_t i = 0; i < ItemCount; ++i)
        {
            m_Select = m_SelectIndex == i;

            // ListBox에서 무언가 선택되면(이전과 똑같은걸 선택하더라도) true 리턴하고
            // 지금 ListBox에서 선택되어 있는게 있다면 m_Select를 true로 만듬
            if (ImGui::Selectable(m_vecItemUTF8[i].c_str(), m_Select))
            {
                // m_SelectIndex는 이전 프레임에서 선택된 ListBox에서 항목인데
                // 이게 이번에 선택된 ListBox에서 항목과 다르고 등록된 콜백이 있다면
                // 콜백 호출하고, m_SelectIndex 갱신
                if (m_SelectIndex != i && m_SelectCallback)
                    m_SelectCallback((int)i, m_vecItem[i].c_str());

                m_SelectIndex = (int)i;
            }

            if (m_Select)
                ImGui::SetItemDefaultFocus();
        }

        ImGui::EndListBox();
    }
}
