#include "IMGUIButton.h"

CIMGUIButton::CIMGUIButton()	:
	m_Click(false)
{
}

CIMGUIButton::~CIMGUIButton()
{
}

bool CIMGUIButton::Init()
{
	return true;
}

void CIMGUIButton::Render()
{
	if (m_Name == "Load Texture")
	{
		//ImGui::Dummy(ImVec2(0.0f, 100.0f));
	}

	m_Click = ImGui::Button(m_Name.c_str(), m_Size);

	if (m_Click && m_ClickCallback)
		m_ClickCallback();
}
