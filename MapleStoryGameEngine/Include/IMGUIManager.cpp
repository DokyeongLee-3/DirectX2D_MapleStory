#include "IMGUIManager.h"
#include "Device.h"
#include "IMGUIWindow.h"
#include "PathManager.h"

DEFINITION_SINGLE(CIMGUIManager)

CIMGUIManager::CIMGUIManager() :
	m_Context(nullptr),
	m_CurrentFont(nullptr)
{

}

CIMGUIManager::~CIMGUIManager()
{
	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		SAFE_DELETE(iter->second);
	}

	ImGui_ImplDX11_Shutdown();
	ImGui_ImplWin32_Shutdown();
	ImGui::DestroyContext();
}

bool CIMGUIManager::Init(HWND hWnd)
{
	m_Context = ImGui::CreateContext();

	ImGuiIO& io = ImGui::GetIO();

	io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
	io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

	// 이걸쓰면 GUI를 화면 밖으로 드래그해서 가져갈 수 있다
	io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

	ImGui::StyleColorsDark();

	// BackEnd Setup
	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(CDevice::GetInst()->GetDevice(), CDevice::GetInst()->GetContext());

	AddFont("Default", "NotoSansKR-Regular.otf", 15.f, true);
	AddFont("DefaultBlack", "NotoSansKR-Black.otf", 15.f, true);
	AddFont("DefaultBold", "NotoSansKR-Bold.otf", 15.f, true);

	m_CurrentFont = FindFont("Default");

	return true;
}

void CIMGUIManager::Update(float DeltaTime)
{
	if (m_mapWindow.empty())
		return;

	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();

	ImGui::NewFrame();

	ImGui::PushFont(m_CurrentFont);

	auto	iter = m_mapWindow.begin();
	auto	iterEnd = m_mapWindow.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->Update(DeltaTime);
	}

	ImGui::PopFont();

	// Window에서 그려낸 위젯들은 실제 화면에 그려지는것이 아니라
	// IMGUI 내부의 버퍼에 그려지게 된다.
	// 이 버퍼를 화면에 최종적으로 한번 그려내야 화면에 보이는데
	// 그걸 ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData())에서 해준다
	// ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData()) 호출전에
	// 그려야 할 것 Widget들(ex. Button)을 IMGUI내부 버퍼에 미리 그려놓는다
	// 그걸 IMGUIWindow::Update에서 윈도우 내의 Widget들이 버퍼에 미리 그려놓도록 한다
	ImGui::Render();
}

void CIMGUIManager::Render()
{
	if (m_mapWindow.empty())
		return;

	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

	ImGuiIO& io = ImGui::GetIO();

	if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
	{
		ImGui::UpdatePlatformWindows();
		// 클라이언트 영역 밖에 나가도 IMGUI가 그려지게 한다
		ImGui::RenderPlatformWindowsDefault();
	}
}

CIMGUIWindow* CIMGUIManager::FindIMGUIWindow(const std::string& Name)
{
	auto	iter = m_mapWindow.find(Name);

	if (iter == m_mapWindow.end())
		return nullptr;

	return iter->second;
}

bool CIMGUIManager::AddFont(const std::string& Name, const char* FileName, float Size, bool Korea, int OverH, int OverV, float Spacing, const std::string& PathName)
{
	ImFont* Font = FindFont(Name);

	if (Font)
		return false;

	char	FullPath[MAX_PATH] = {};

	const PathInfo* Info = CPathManager::GetInst()->FindPath(PathName);

	if (Info)
		strcpy_s(FullPath, Info->PathMultibyte);

	strcat_s(FullPath, FileName);

	return AddFontFullPath(Name, FullPath, Size, Korea, OverH, OverV, Spacing);
}

bool CIMGUIManager::AddFontFullPath(const std::string& Name, const char* FullPath, float Size, bool Korea, int OverH, int OverV, float Spacing)
{
	ImFont* Font = FindFont(Name);

	if (Font)
		return false;

	ImGuiIO& io = ImGui::GetIO();

	ImFontConfig	config;

	// OverSample* 옵션은 rasterizer 단계에서 subpixel을 몇개써서
	// 얼마나 좋은 퀄리티로 폰트를 만들것인지
	// 2아니면 3으로 쓰는데 차이가 없어서 2로하면 됨
	config.OversampleH = OverH;
	config.OversampleV = OverV;
	config.GlyphExtraSpacing.x = Spacing;
	config.PixelSnapH = 1;

	if (!Korea)
		Font = io.Fonts->AddFontFromFileTTF(FullPath, Size, &config, io.Fonts->GetGlyphRangesDefault());

	else
		Font = io.Fonts->AddFontFromFileTTF(FullPath, Size, &config, io.Fonts->GetGlyphRangesKorean());

	m_mapFont.insert(std::make_pair(Name, Font));

	return true;
}

void CIMGUIManager::SetCurrentFont(const std::string& Name)
{
	ImFont* Font = FindFont(Name);

	if (!Font)
		return;

	m_CurrentFont = Font;
}

void CIMGUIManager::SetCurrentFont()
{
	ImGui::PushFont(m_CurrentFont);
}

void CIMGUIManager::ResetCurrentFont()
{
	ImGui::PopFont();
}

ImFont* CIMGUIManager::FindFont(const std::string& Name)
{
	auto	iter = m_mapFont.find(Name);

	if (iter == m_mapFont.end())
		return nullptr;

	return iter->second;
}
