#include "IMGUILabel.h"

CIMGUILabel::CIMGUILabel()
{
    m_Align = ImVec2(0.f, 0.f);
}

CIMGUILabel::~CIMGUILabel()
{
}

bool CIMGUILabel::Init()
{
    return true;
}

void CIMGUILabel::Render()
{
    ImGui::PushStyleVar(ImGuiStyleVar_ButtonTextAlign, m_Align);

    ImGui::PushStyleColor(ImGuiCol_Button, m_Color.Value);
    // ��ư�� Label�� ����ϱ� ���� ��ư���� ���콺�� �ö󰡰ų�
    // Ŭ���� �ص� ������ �Ȱ��� �ؼ� ��ư�� �ƴ϶� Labeló�� ���� �� �ְ� �Ѵ�
    ImGui::PushStyleColor(ImGuiCol_ButtonHovered, m_Color.Value);
    ImGui::PushStyleColor(ImGuiCol_ButtonActive, m_Color.Value);

    ImGui::Button(m_Name.c_str(), m_Size);

    // ������ PushStyleColor�� 3�� �־������Ƿ� PopStyleColor ���ڷ� 3
    ImGui::PopStyleColor(3);
    ImGui::PopStyleVar();

    if (m_Name == "TileMapInfo")
    {
        ImVec2 RectMin = ImGui::GetItemRectMin();
        ImVec2 LabelSize = m_Size;


        ImDrawList* draw_list = ImGui::GetWindowDrawList();

        static ImVec4 colf = ImVec4(0.3f, 0.3f, 0.3f, 0.9f);
        const ImU32 col = ImColor(colf);

        draw_list->AddLine(ImVec2(RectMin.x + LabelSize.x + 6.f, RectMin.y), ImVec2(RectMin.x + LabelSize.x + 6.f, RectMin.y + 225.f), col, 1.6f);
        

    }
}
