#pragma once

#include "GameInfo.h"

//  If you use this binding you'll need to call 4 functions:
// ImGui_ImplXXXX_Init(), ImGui_ImplXXXX_NewFrame(), 
// ImGui::Render() and ImGui_ImplXXXX_Shutdown().
class CIMGUIManager
{
private:
	ImGuiContext* m_Context;
	std::unordered_map<std::string, class CIMGUIWindow*>	m_mapWindow;
	std::unordered_map<std::string, ImFont*>	m_mapFont;
	ImFont* m_CurrentFont;

public:
	bool Init(HWND hWnd);
	void Update(float DeltaTime);
	void Render();
	class CIMGUIWindow* FindIMGUIWindow(const std::string& Name);

public:
	ImFont* GetCurrentFont()	const
	{
		return m_CurrentFont;
	}

public:
	bool AddFont(const std::string& Name, const char* FileName, float Size, bool Korea = false, int OverH = 2, int OverV = 1,
		float Spacing = 1.f, const std::string& PathName = FONT_PATH);
	bool AddFontFullPath(const std::string& Name, const char* FullPath, float Size, bool Korea = false, int OverH = 2, int OverV = 1,
		float Spacing = 1.f);
	void SetCurrentFont(const std::string& Name);
	void SetCurrentFont();
	void ResetCurrentFont();
	ImFont* FindFont(const std::string& Name);

public:
	template <typename T>
	T* AddWindow(const std::string& Name)
	{
		T* Window = (T*)FindIMGUIWindow(Name);

		if (Window)
			return Window;

		Window = new T;

		Window->SetName(Name);
		if (!Window->Init())
		{
			SAFE_DELETE(Window);
			return nullptr;
		}

		m_mapWindow.insert(std::make_pair(Name, Window));

		return Window;
	}

	DECLARE_SINGLE(CIMGUIManager)
};

