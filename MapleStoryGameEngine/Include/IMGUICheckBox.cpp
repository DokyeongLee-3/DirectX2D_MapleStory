#include "IMGUICheckBox.h"

CIMGUICheckBox::CIMGUICheckBox()    :
    m_Check(false)
{
}

CIMGUICheckBox::~CIMGUICheckBox()
{
}

bool CIMGUICheckBox::Init()
{
    return true;
}

void CIMGUICheckBox::Render()
{
    if (ImGui::Checkbox(m_TextUTF8, &m_Check))
    {
        if (m_Check && m_Callback)
            m_Callback();
    }
}
