#include "IMGUIText.h"

CIMGUIText::CIMGUIText() :
    m_Text{},
    m_wText{},
    m_TextUTF8{}
{
    m_Color.Value.x = 1.f;
    m_Color.Value.y = 1.f;
    m_Color.Value.z = 1.f;
    m_Color.Value.w = 1.f;
}

CIMGUIText::~CIMGUIText()
{
}

bool CIMGUIText::Init()
{
    return true;
}

void CIMGUIText::Render()
{
    ImGui::TextColored(m_Color.Value, m_TextUTF8);
}
