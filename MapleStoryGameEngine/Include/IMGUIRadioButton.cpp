#include "IMGUIRadioButton.h"
#include "Device.h"

CIMGUIRadioButton::CIMGUIRadioButton()
{
}

CIMGUIRadioButton::~CIMGUIRadioButton()
{
}

bool CIMGUIRadioButton::Init()
{
	return true;
}

void CIMGUIRadioButton::Render()
{
    static int Radio = 0;

    int Size = (int)m_vecRadioData.size();

    for (int i = 0; i < Size; ++i)
    {
        //bool Click = ImGui::RadioButton(m_vecRadioData[i].TextUTF8, &Radio, i); 

        bool Active = m_vecActive[i];

        bool Click = ImGui::RadioButton(m_vecRadioData[i].TextUTF8, Active);
        if(i != Size - 1)
            ImGui::SameLine();


        if(Click)
        {
            if (m_vecRadioData[i].Callback)
                m_vecRadioData[i].Callback();


            if (i == Size - 1)
                break;

        }
    }

}
