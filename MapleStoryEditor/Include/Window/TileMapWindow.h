#pragma once
#include "IMGUIWindow.h"
#include "../EditorInfo.h"
#include "Component/TileMapComponent.h"

class CTileMapWindow :
    public CIMGUIWindow
{
public:
	CTileMapWindow();
	virtual ~CTileMapWindow();

private:
	class CIMGUITextInput* m_CountX;
	class CIMGUITextInput* m_CountY;
	class CIMGUITextInput* m_SizeX;
	class CIMGUITextInput* m_SizeY;
	class CIMGUIComboBox* m_ShapeCombo;
	class CIMGUIButton* m_TileMapCreateButton;
	CSharedPtr<CTileMapComponent>	m_TileMap;

private:
	class CIMGUIComboBox* m_TypeCombo;
	class CIMGUIComboBox* m_TileEditCombo;
	class CIMGUITextInput* m_TileLeftmost;
	class CIMGUITextInput* m_TileTopmost;
	class CIMGUITextInput* m_FrameStartX;
	class CIMGUITextInput* m_FrameStartY;
	class CIMGUITextInput* m_FrameEndX;
	class CIMGUITextInput* m_FrameEndY;
	class CIMGUIButton* m_DefaultFrameButton;

	class CIMGUIImage* m_TileImage;
	class CIMGUIImage* m_TileSprite;
	//class CIMGUIButton* m_TileMaterialSelectButton;

	//bool m_HoverTileRender;
	//ImVec2 m_HoverTileStartPos;
	//ImVec2 m_HoverTileEndPos;
	//ImVec4 m_HoverTileColor;

//private:
//	class CIMGUIButton* m_TileMapSaveButton;
//	class CIMGUIButton* m_TileMapLoadButton;

public:
	void SetTileMap(CTileMapComponent* TileMap)
	{
		m_TileMap = TileMap;
	}

public:
	virtual bool Init();
	virtual void Update(float DeltaTime);

private:
	void CountXCallback();
	void CountYCallback();
	void SizeXCallback();
	void SizeYCallback();
	void TileMapCreateButton();

private:
	void DefaultFrameButton();


private:
	void CreateTileEditControl();
	void SelectTileMaterial();

public:
	void TileImageHoverCallback();

//private:
//	void TileMapSaveButton();
//	void TileMapLoadButton();
};

