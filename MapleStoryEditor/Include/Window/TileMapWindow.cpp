
#include "TileMapWindow.h"
#include "IMGUIButton.h"
#include "IMGUISameLine.h"
#include "IMGUILabel.h"
#include "IMGUIText.h"
#include "IMGUITextInput.h"
#include "IMGUIListBox.h"
#include "IMGUIComboBox.h"
#include "Scene/SceneManager.h"
#include "Scene/Scene.h"
#include "Scene/SceneResource.h"
#include "Resource/Texture/Texture.h"

CTileMapWindow::CTileMapWindow()
{
}

CTileMapWindow::~CTileMapWindow()
{
}

bool CTileMapWindow::Init()
{
	CIMGUIWindow::Init();

	CIMGUILabel* Label = AddWidget<CIMGUILabel>("TileMapInfo", 320.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	//Label = AddWidget<CIMGUILabel>("TileShape", 300.f, 30.f);

	//Label->SetColor(128, 128, 128);
	//Label->SetAlign(0.5f, 0.f);


	m_ShapeCombo = AddWidget<CIMGUIComboBox>("TileShape", 130.f, 30.f);

	m_ShapeCombo->SetHideName(true);
	m_ShapeCombo->AddItem("사각형");
	m_ShapeCombo->AddItem("마름모");


	Label = AddWidget<CIMGUILabel>("CountX", 100.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_CountX = AddWidget<CIMGUITextInput>("CountX", 100.f, 30.f);
	m_CountX->SetHideName(true);
	m_CountX->SetTextType(ImGuiText_Type::Int);
	m_CountX->SetCallback(this, &CTileMapWindow::CountXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("CountY", 100.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_CountY = AddWidget<CIMGUITextInput>("CountY", 100.f, 30.f);
	m_CountY->SetHideName(true);
	m_CountY->SetTextType(ImGuiText_Type::Int);
	m_CountY->SetCallback(this, &CTileMapWindow::CountYCallback);

	Label = AddWidget<CIMGUILabel>("SizeX", 100.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SizeX = AddWidget<CIMGUITextInput>("SizeX", 100.f, 30.f);
	m_SizeX->SetHideName(true);
	m_SizeX->SetTextType(ImGuiText_Type::Float);
	m_SizeX->SetCallback(this, &CTileMapWindow::SizeXCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("SizeY", 100.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_SizeY = AddWidget<CIMGUITextInput>("SizeY", 100.f, 30.f);
	m_SizeY->SetHideName(true);
	m_SizeY->SetTextType(ImGuiText_Type::Float);
	m_SizeY->SetCallback(this, &CTileMapWindow::SizeYCallback);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Line->SetOffsetX(500.f);

	m_TileMapCreateButton = AddWidget<CIMGUIButton>("TileMapCreateButton", 150.f, 30.f);

	m_TileMapCreateButton->SetClickCallback(this, &CTileMapWindow::TileMapCreateButton);


	CreateTileEditControl();

	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);
}

void CTileMapWindow::CountXCallback()
{
}

void CTileMapWindow::CountYCallback()
{
}

void CTileMapWindow::SizeXCallback()
{
}

void CTileMapWindow::SizeYCallback()
{
}

void CTileMapWindow::TileMapCreateButton()
{
	if (!m_TileMap)
		return;

	int	ShapeIndex = m_ShapeCombo->GetSelectIndex();

	if (ShapeIndex >= (int)Tile_Shape::End || ShapeIndex < 0)
		return;

	int	CountX, CountY;

	CountX = m_CountX->GetValueInt();
	CountY = m_CountY->GetValueInt();

	Vector3	Size;
	Size.x = m_SizeX->GetValueFloat();
	Size.y = m_SizeY->GetValueFloat();

	m_TileMap->CreateTile((Tile_Shape)ShapeIndex, CountX, CountY, Size);

	CMaterial* Material = m_TileMap->GetTileMaterial();
	CTexture* Texture = nullptr;

	if ((Tile_Shape)ShapeIndex == Tile_Shape::Rect)
	{
		CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRectTile",
			TEXT("Floors.png"));

		Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRectTile");
	}

	else
	{
		CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRhombusTile",
			TEXT("Diablos_Lair_Floor_TRS/Diablos_Lair_Floor.png"));

		Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture("DefaultRhombusTile");
	}

	if (Material->EmptyTexture())
		Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);

	else
		Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);
}

void CTileMapWindow::DefaultFrameButton()
{
	float	StartX, StartY, EndX, EndY;

	StartX = m_FrameStartX->GetValueFloat();
	StartY = m_FrameStartY->GetValueFloat();
	EndX = m_FrameEndX->GetValueFloat();
	EndY = m_FrameEndY->GetValueFloat();

	m_TileMap->SetTileDefaultFrame(StartX, StartY, EndX, EndY);
}

void CTileMapWindow::CreateTileEditControl()
{
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("TileInfo", 320.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	m_TypeCombo = AddWidget<CIMGUIComboBox>("TileType", 130.f, 30.f);

	m_TypeCombo->SetHideName(true);
	m_TypeCombo->AddItem("일반");
	m_TypeCombo->AddItem("벽");

	Label = AddWidget<CIMGUILabel>("FrameStartX", 130.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	m_FrameStartX = AddWidget<CIMGUITextInput>("FrameStartX", 100.f, 30.f);
	m_FrameStartX->SetHideName(true);
	m_FrameStartX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("FrameStartY", 130.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_FrameStartY = AddWidget<CIMGUITextInput>("FrameStartY", 100.f, 30.f);
	m_FrameStartY->SetHideName(true);
	m_FrameStartY->SetTextType(ImGuiText_Type::Float);


	Label = AddWidget<CIMGUILabel>("FrameEndX", 130.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_FrameEndX = AddWidget<CIMGUITextInput>("FrameEndX", 100.f, 30.f);
	m_FrameEndX->SetHideName(true);
	m_FrameEndX->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("FrameEndY", 130.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_FrameEndY = AddWidget<CIMGUITextInput>("FrameEndY", 100.f, 30.f);
	m_FrameEndY->SetHideName(true);
	m_FrameEndY->SetTextType(ImGuiText_Type::Float);

	Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);
	Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Line->SetOffsetX(500.f);

	m_DefaultFrameButton = AddWidget<CIMGUIButton>("DefaultFrameButton", 150.f, 30.f);

	m_DefaultFrameButton->SetClickCallback(this, &CTileMapWindow::DefaultFrameButton);
}
