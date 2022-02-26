
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
#include "../EditorManager.h"
#include "Input.h"
#include "Component/Tile.h"
#include "IMGUIImage.h"
#include "IMGUIManager.h"
#include "DetailWindow.h"
#include "PathManager.h"

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

	Label = AddWidget<CIMGUILabel>("", 40.f, 30.f);
	Label->SetColorFloat(0.0f, 0.0f, 0.0f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Line->SetOffsetX(500.f);

	m_TileMapCreateButton = AddWidget<CIMGUIButton>("TileMapCreateButton", 150.f, 30.f);

	m_TileMapCreateButton->SetClickCallback(this, &CTileMapWindow::TileMapCreateButton);

	CreateTileEditControl();

	m_TileImage = AddWidget<CIMGUIImage>("TileImage", 200.f, 200.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_TileSprite = AddWidget<CIMGUIImage>("TileSprite", 200.f, 200.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	Line->SetOffsetX(500.f);

	m_TileMaterialSelectButton = AddWidget<CIMGUIButton>("Select Material", 150.f, 30.f);
	m_TileMaterialSelectButton->SetClickCallback<CTileMapWindow>(this, &CTileMapWindow::SelectTileMaterial);





	//m_TileMapSaveButton = AddWidget<CIMGUIButton>("TileMapSaveButton", 150.f, 30.f);

	//m_TileMapSaveButton->SetClickCallback(this, &CTileMapWindow::TileMapSaveButton);

	//m_TileMapLoadButton = AddWidget<CIMGUIButton>("TileMapLoadButton", 150.f, 30.f);

	//m_TileMapLoadButton->SetClickCallback(this, &CTileMapWindow::TileMapLoadButton);


	return true;
}

void CTileMapWindow::Update(float DeltaTime)
{
	CIMGUIWindow::Update(DeltaTime);

	if (CEditorManager::GetInst()->GetEditMode() == EditMode::TileMap && m_TileMap)
	{
		m_TileMap->EnableEditMode(true);

		if (CEditorManager::GetInst()->GetLButtonPush())
		{
			// 마우스 위치를 얻어와서 어떤 타일인지를 구한다.
			Vector2 MouseWorldPos = CInput::GetInst()->GetMouseWorld2DPos();

			CTile* Tile = m_TileMap->GetTile(Vector3(MouseWorldPos.x, MouseWorldPos.y, 0.f));

			if (Tile)
			{
				int	TileEditMode = m_TileEditCombo->GetSelectIndex();

				switch ((TileEdit_Mode)TileEditMode)
				{
				case TileEdit_Mode::Type:
				{
					int	TileType = m_TypeCombo->GetSelectIndex();

					if (TileType == -1)
						return;

					Tile_Type	Type = (Tile_Type)TileType;

					Tile->SetTileType(Type);
				}
				break;
				case TileEdit_Mode::Frame:
				{
					float	StartX, StartY, EndX, EndY;

					StartX = m_FrameStartX->GetValueFloat();
					StartY = m_FrameStartY->GetValueFloat();
					EndX = m_FrameEndX->GetValueFloat();
					EndY = m_FrameEndY->GetValueFloat();

					Tile->SetFrameStart(StartX, StartY);
					Tile->SetFrameEnd(EndX, EndY);
				}
				break;
				}
			}
		}
	}
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

	Vector2 Scale = Vector2(CountX * Size.x, CountY * Size.y);

	m_TileMap->CreateTile((Tile_Shape)ShapeIndex, CountX, CountY, Size);
	m_TileMap->SetWorldScale(Scale.x, Scale.y, 0.f);

	CDetailWindow* DetailWindow = (CDetailWindow*)CIMGUIManager::GetInst()->FindIMGUIWindow("DetailWindow");

	DetailWindow->SetScaleXInput(Scale.x);
	DetailWindow->SetScaleXInput(Scale.y);

	CMaterial* Material = m_TileMap->GetTileMaterial();
	CTexture* Texture = nullptr;

	if ((Tile_Shape)ShapeIndex == Tile_Shape::Rect)
	{
		CSceneManager::GetInst()->GetScene()->GetResource()->LoadTexture("DefaultRectTile",
			TEXT("AllTile.png"));

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

	m_TileImage->SetTexture(Texture);
	m_TileSprite->SetTexture(Texture);

}

void CTileMapWindow::DefaultFrameButton()
{
	if (!m_TileMap)
		return;

	float	StartX, StartY, EndX, EndY;

	StartX = m_FrameStartX->GetValueFloat();
	StartY = m_FrameStartY->GetValueFloat();
	EndX = m_FrameEndX->GetValueFloat();
	EndY = m_FrameEndY->GetValueFloat();

	m_TileMap->SetTileDefaultFrame(StartX, StartY, EndX, EndY);

	m_TileSprite->SetImageStart(StartX, StartY);
	m_TileSprite->SetImageEnd(EndX, EndY);
	m_TileSprite->SetSize(Vector2(EndX - StartX, EndY - StartY));
}

void CTileMapWindow::CreateTileEditControl()
{
	CIMGUILabel* Label = AddWidget<CIMGUILabel>("TileInfo", 320.f, 30.f);

	Label->SetColor(80, 80, 80);
	Label->SetAlign(0.5f, 0.f);

	m_TileEditCombo = AddWidget<CIMGUIComboBox>("TileEdit", 130.f, 30.f);

	m_TileEditCombo->SetHideName(true);
	m_TileEditCombo->AddItem("Type");
	m_TileEditCombo->AddItem("Frame");

	CIMGUISameLine* Line = AddWidget<CIMGUISameLine>("Line");

	Label = AddWidget<CIMGUILabel>("", 100.f, 30.f);

	Label->SetColor(0, 0, 0);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

	m_TypeCombo = AddWidget<CIMGUIComboBox>("TileType", 130.f, 30.f);

	m_TypeCombo->SetHideName(true);
	m_TypeCombo->AddItem("Normal");
	m_TypeCombo->AddItem("Wall");

	Label = AddWidget<CIMGUILabel>("FrameStartX", 130.f, 30.f);

	Label->SetColor(128, 128, 128);
	Label->SetAlign(0.5f, 0.f);

	Line = AddWidget<CIMGUISameLine>("Line");

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

	Line->SetOffsetX(500.f);

	m_DefaultFrameButton = AddWidget<CIMGUIButton>("DefaultFrameButton", 150.f, 30.f);

	m_DefaultFrameButton->SetClickCallback(this, &CTileMapWindow::DefaultFrameButton);
}

void CTileMapWindow::SelectTileMaterial()
{
	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0DDSFile\0*.dds\0TGAFile\0*.tga\0PNGFile\0*.png\0JPGFile\0*.jpg\0JPEGFile\0*.jpeg\0BMPFile\0*.bmp");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(TEXTURE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		TCHAR   FileName[MAX_PATH] = {};
		TCHAR   FileExt[MAX_PATH] = {};

		_wsplitpath_s(FilePath, 0, 0, 0, 0, FileName, MAX_PATH, FileExt, MAX_PATH);

		char    ConvertFileName[MAX_PATH] = {};
		char    ConvertFileExt[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FileName, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileName, -1, ConvertFileName, Length, 0, 0);

		Length = WideCharToMultiByte(CP_ACP, 0, FileExt, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FileExt, -1, ConvertFileExt, Length, 0, 0);


		//m_TileSprite->SetTextureFullPath(0, 0, (int)Buffer_Shader_Type::Pixel, ConvertFileName, FilePath);
		CMaterial* Material = m_TileMap->GetTileMaterial();
		CTexture* Texture = nullptr;

		CSceneManager::GetInst()->GetScene()->GetResource()->LoadTextureFullPath(ConvertFileName, FilePath);

		Texture = CSceneManager::GetInst()->GetScene()->GetResource()->FindTexture(ConvertFileName);

		if (Material->EmptyTexture())
			Material->AddTexture(0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);

		else
			Material->SetTexture(0, 0, (int)Buffer_Shader_Type::Pixel, "TileTexture", Texture);

		m_TileSprite->SetTexture(Texture);
	}
}

void CTileMapWindow::TileMapSaveButton()
{
	if (!m_TileMap)
		return;

	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0GameObject File\0*.gobj");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

	if (GetSaveFileName(&OpenFile) != 0)
	{
		char    ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);

		CGameObject* TileMapObj = m_TileMap->GetGameObject();

		TileMapObj->Save(ConvertFullPath);

		//CSceneManager::GetInst()->GetScene()->SaveFullPath(ConvertFullPath);
	}
}

void CTileMapWindow::TileMapLoadButton()
{
	if (!m_TileMap)
		return;

	TCHAR   FilePath[MAX_PATH] = {};

	OPENFILENAME    OpenFile = {};

	OpenFile.lStructSize = sizeof(OPENFILENAME);
	OpenFile.hwndOwner = CEngine::GetInst()->GetWindowHandle();
	OpenFile.lpstrFilter = TEXT("모든파일\0*.*\0GameObject File\0*.gobj");
	OpenFile.lpstrFile = FilePath;
	OpenFile.nMaxFile = MAX_PATH;
	OpenFile.lpstrInitialDir = CPathManager::GetInst()->FindPath(SCENE_PATH)->Path;

	if (GetOpenFileName(&OpenFile) != 0)
	{
		char    ConvertFullPath[MAX_PATH] = {};

		int Length = WideCharToMultiByte(CP_ACP, 0, FilePath, -1, 0, 0, 0, 0);
		WideCharToMultiByte(CP_ACP, 0, FilePath, -1, ConvertFullPath, Length, 0, 0);


		CGameObject* TileMapObj = m_TileMap->GetGameObject();

		TileMapObj->Load(ConvertFullPath);
		//CSceneManager::GetInst()->GetScene()->LoadFullPath(ConvertFullPath);
	}
}