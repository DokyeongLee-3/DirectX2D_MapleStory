
#include "IMGUIWindow.h"
#include "IMGUIWidget.h"

CIMGUIWindow::CIMGUIWindow()    :
    m_Open(true),
    m_WindowFlag(0),
    m_ModalPopup(false)
{
}

CIMGUIWindow::~CIMGUIWindow()
{
	size_t	Size = m_vecPopupWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecPopupWidget[i]);
	}

	m_vecPopupWidget.clear();

	Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecWidget[i]);
	}

	m_vecWidget.clear();
}

void CIMGUIWindow::Open()
{
	m_Open = true;
}

void CIMGUIWindow::Close()
{
	m_Open = false;
}

void CIMGUIWindow::ClosePopup()
{
	ImGui::CloseCurrentPopup();

	size_t	Size = m_vecPopupWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecPopupWidget[i]);
	}

	m_vecPopupWidget.clear();
}

CIMGUIWidget* CIMGUIWindow::FindWidget(const std::string& Name)
{
	size_t Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		if (m_vecWidget[i]->m_Name == Name)
			return m_vecWidget[i];
	}

	return nullptr;
}

bool CIMGUIWindow::Init()
{
    return true;
}

void CIMGUIWindow::Update(float DeltaTime)
{
	if (!m_Open)
		return;

	// IMGUI창을 열어주는 함수, 닫혀있으면 false를 리턴
	// 세모 화살표를 누르면 ImGui::Begin이 false를 리턴해서 m_Open을 fals로 만듬
	// 여러 기능들(ex. Menu Bar)을 WindowFlag로 추가할 수 있다
	if (!ImGui::Begin(m_Name.c_str(), &m_Open, m_WindowFlag))
		m_Open = false;

	size_t Size = m_vecWidget.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecWidget[i]->Render();
	}

	ImGui::End();
}
