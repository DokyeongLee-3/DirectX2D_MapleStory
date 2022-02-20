#pragma once



#if defined(_MSC_VER) && !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif

#include "imgui.h"
#ifndef IMGUI_DISABLE
#endif

#ifndef IMGUI_DEFINE_MATH_OPERATORS
#define IMGUI_DEFINE_MATH_OPERATORS
#endif
#include "imgui_internal.h"

// System includes
#include <ctype.h>      // toupper
#if defined(_MSC_VER) && _MSC_VER <= 1500 // MSVC 2008 or earlier
#include <stddef.h>     // intptr_t
#else
#include <stdint.h>     // intptr_t
#endif

#include "IMGUIWidget.h"

struct RadioData
{
	char TextUTF8[1024];
	std::function<void()> Callback;
};

class CIMGUIRadioButton :
    public CIMGUIWidget
{
	friend class CIMGUIWindow;
	friend class CIMGUITree;

protected:
	CIMGUIRadioButton();
	virtual ~CIMGUIRadioButton();

protected:
	std::vector<RadioData> m_vecRadioData;
    std::vector<bool>       m_vecActive;

public:
    char* GetRadioButtonLabel(int Index)
    {
        return m_vecRadioData[Index].TextUTF8;
    }

    void AddActive(bool Active)
    {
        m_vecActive.push_back(Active);
    }

    void SetActive(int Index, bool Active)
    {
        m_vecActive[Index] = Active;
    }

    void CallRadioButtonCallback(int Index)
    {
        m_vecRadioData[Index].Callback();
    }

	template <typename T>
	void AddText(const char* Text, T* Obj, void(T::* Func)())
	{
		int TextLen = (int)strlen(Text);
		wchar_t	wText[1024] = {};

		int	Length = MultiByteToWideChar(CP_ACP, 0, Text, -1, 0, 0);
		MultiByteToWideChar(CP_ACP, 0, Text, -1, wText, Length);

		// UTF8로 변환한다.
		Length = WideCharToMultiByte(CP_UTF8, 0, wText, -1, 0, 0, 0, 0);

		char* TextUTF8 = new char[Length + 1];

		WideCharToMultiByte(CP_UTF8, 0, wText, -1, TextUTF8, Length, 0, 0);

		RadioData Data;

		strcpy_s(Data.TextUTF8, TextUTF8);

		if (Obj && Func)
			Data.Callback = std::bind(Func, Obj);
		else
			Data.Callback = nullptr;


		m_vecRadioData.push_back(Data);

		delete[] TextUTF8;
	}

public:
	virtual bool Init();
	virtual void Render();

    //static bool ForceRadioButtonCheck(const char* label, bool active)
    //{
    //    ImGuiWindow* window = ImGui::GetCurrentWindow();
    //    if (window->SkipItems)
    //        return false;

    //    ImGuiContext& g = *GImGui;
    //    const ImGuiStyle& style = g.Style;
    //    const ImGuiID id = window->GetID(label);
    //    const ImVec2 label_size = ImGui::CalcTextSize(label, NULL, true);

    //    const float square_sz = ImGui::GetFrameHeight();
    //    const ImVec2 pos = window->DC.CursorPos;
    //    const ImRect check_bb(pos, pos + ImVec2(square_sz, square_sz));
    //    const ImRect total_bb(pos, pos + ImVec2(square_sz + (label_size.x > 0.0f ? style.ItemInnerSpacing.x + label_size.x : 0.0f), label_size.y + style.FramePadding.y * 2.0f));
    //    ImGui::ItemSize(total_bb, style.FramePadding.y);
    //    if (!ImGui::ItemAdd(total_bb, id))
    //        return false;

    //    ImVec2 center = check_bb.GetCenter();
    //    center.x = IM_ROUND(center.x);
    //    center.y = IM_ROUND(center.y);
    //    const float radius = (square_sz - 1.0f) * 0.5f;

    //    bool hovered, held;
    //    bool pressed = ImGui::ButtonBehavior(total_bb, id, &hovered, &held);
    //    if (!pressed)
    //        ImGui::MarkItemEdited(id);

    //    ImGui::RenderNavHighlight(total_bb, id);
    //    window->DrawList->AddCircleFilled(center, radius, ImGui::GetColorU32((held && hovered) ? ImGuiCol_FrameBgActive : hovered ? ImGuiCol_FrameBgHovered : ImGuiCol_FrameBg), 16);
    //    if (active)
    //    {
    //        const float pad = ImMax(1.0f, IM_FLOOR(square_sz / 6.0f));
    //        window->DrawList->AddCircleFilled(center, radius - pad, ImGui::GetColorU32(ImGuiCol_CheckMark), 16);
    //    }

    //    //if (style.FrameBorderSize > 0.0f)
    //    // {
    //        window->DrawList->AddCircle(center + ImVec2(1, 1), radius, ImGui::GetColorU32(ImGuiCol_BorderShadow), 16, style.FrameBorderSize);
    //        window->DrawList->AddCircle(center, radius, ImGui::GetColorU32(ImGuiCol_Border), 16, style.FrameBorderSize);
    //    //}

    //    ImVec2 label_pos = ImVec2(check_bb.Max.x + style.ItemInnerSpacing.x, check_bb.Min.y + style.FramePadding.y);
    //    if (g.LogEnabled)
    //        ImGui::LogRenderedText(&label_pos, active ? "(x)" : "( )");
    //    if (label_size.x > 0.0f)
    //        ImGui::RenderText(label_pos, label);

    //    IMGUI_TEST_ENGINE_ITEM_INFO(id, label, g.LastItemData.StatusFlags);
    //    return pressed;
    //}

};

