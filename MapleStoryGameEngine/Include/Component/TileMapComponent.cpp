
#include "TileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Animation/AnimationSequence2DInstance.h"
#include "../Render/RenderManager.h"
#include "../Resource/Shader/Standard2DConstantBuffer.h"
#include "../Scene/SceneManager.h"

CTileMapComponent::CTileMapComponent()
{
	SetTypeID<CTileMapComponent>();
	m_Render = true;

	m_CountX = 0;
	m_CountY = 0;
	m_RenderCount = 0;
	m_TileShape = Tile_Shape::Rect;
	m_LayerName = "Back";
}

CTileMapComponent::CTileMapComponent(const CTileMapComponent& com) :
	CSceneComponent(com)
{
	m_BackMesh = com.m_BackMesh;

	if (com.m_BackMaterial)
		m_BackMaterial = com.m_BackMaterial->Clone();
}

CTileMapComponent::~CTileMapComponent()
{
}

void CTileMapComponent::SetMaterial(CMaterial* Material)
{
	m_BackMaterial = Material->Clone();

	m_BackMaterial->SetScene(m_Scene);
}

void CTileMapComponent::SetBackBaseColor(const Vector4& Color)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(Color);
}

void CTileMapComponent::SetBackBaseColor(float r, float g, float b, float a)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetBaseColor(r, g, b, a);
}

void CTileMapComponent::SetBackRenderState(CRenderState* State)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(State);
}

void CTileMapComponent::SetBackRenderState(const std::string& Name)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetRenderState(Name);
}

void CTileMapComponent::SetBackTransparency(bool Enable)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTransparency(Enable);
}

void CTileMapComponent::SetBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetOpacity(Opacity);
}

void CTileMapComponent::AddBackOpacity(float Opacity)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddOpacity(Opacity);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, Texture);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, FileName, PathName);
}

void CTileMapComponent::AddBackTextureFullPath(int Register, int ShaderType, const std::string& Name, const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTextureFullPath(Register, ShaderType, Name, FullPath);
}

void CTileMapComponent::AddBackTexture(int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->AddTexture(Register, ShaderType, Name, vecFileName, PathName);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name, CTexture* Texture)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, Texture);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name, const TCHAR* FileName,
	const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, FileName, PathName);
}

void CTileMapComponent::SetBackTextureFullPath(int Index, int Register, int ShaderType, const std::string& Name,
	const TCHAR* FullPath)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTextureFullPath(Index, Register, ShaderType, Name, FullPath);
}

void CTileMapComponent::SetBackTexture(int Index, int Register, int ShaderType, const std::string& Name,
	const std::vector<TCHAR*>& vecFileName, const std::string& PathName)
{
	if (!m_BackMaterial)
		return;

	m_BackMaterial->SetTexture(Index, Register, ShaderType, Name, vecFileName, PathName);
}

void CTileMapComponent::CreateTile(Tile_Shape Shape, int CountX, int CountY, const Vector3& Size)
{
	m_TileShape = Shape;
	m_CountX = CountX;
	m_CountY = CountY;
	m_TileSize = Size;

	m_vecTile.resize(m_CountX * m_CountY);

	for (int i = 0; i < m_CountY; ++i)
	{
		for (int j = 0; j < m_CountX; ++j)
		{
			CTile* Tile = new CTile;

			Tile->m_Owner = this;

			int	Index = i * m_CountX + j;

			Tile->SetIndex(j, i, Index);
			Tile->SetSize(m_TileSize);
			Tile->SetShape(m_TileShape);

			m_vecTile[Index] = Tile;
		}
	}

	switch (m_TileShape)
	{
	case Tile_Shape::Rect:
	{
		Vector3	Pos;

		for (int i = 0; i < m_CountY; ++i)
		{
			Pos.x = 0.f;
			Pos.y = i * m_TileSize.y;

			for (int j = 0; j < m_CountX; ++j)
			{
				Pos.x = j * m_TileSize.x;

				int	Index = i * m_CountX + j;

				m_vecTile[Index]->SetPos(Pos);
			}
		}
	}
	break;
	case Tile_Shape::Rhombus:
	{
		Vector3	Pos;

		for (int i = 0; i < m_CountY; ++i)
		{
			if (i % 2 == 0)
				Pos.x = 0.f;

			else
				Pos.x = m_TileSize.x * 0.5f;

			Pos.y = i * m_TileSize.y * 0.5f;

			for (int j = 0; j < m_CountX; ++j)
			{
				if (j > 0.f)
					Pos.x += m_TileSize.x;

				int	Index = i * m_CountX + j;

				m_vecTile[Index]->SetPos(Pos);
			}
		}
	}
	break;
	}

	//m_CBuffer->SetTileSize(Vector2(m_TileSize.x, m_TileSize.y));
}

int CTileMapComponent::GetTileRenderIndexX(const Vector3& Pos)
{
	if (m_TileShape == Tile_Shape::Rect)
	{
		float	ConvertX = Pos.x - GetWorldPos().x;

		int	IndexX = (int)(ConvertX / m_TileSize.x);

		if (IndexX < 0)
			return 0;

		else if (IndexX >= m_CountX)
			return m_CountX - 1;

		return IndexX;
	}

	return 0;
}

int CTileMapComponent::GetTileRenderIndexY(const Vector3& Pos)
{
	if (m_TileShape == Tile_Shape::Rect)
	{
		float	ConvertY = Pos.y - GetWorldPos().y;

		int	IndexY = (int)(ConvertY / m_TileSize.y);

		if (IndexY < 0)
			return 0;

		else if (IndexY >= m_CountY)
			return m_CountY - 1;

		return IndexY;
	}

	return 0;
}

void CTileMapComponent::Start()
{
	CSceneComponent::Start();
}

bool CTileMapComponent::Init()
{
	m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");
	//SetMaterial(m_Scene->GetResource()->FindMaterial("BaseTexture"));

	SetMeshSize(1.f, 1.f, 0.f);
	//SetWorldScale((float)m_BackMaterial->GetTextureWidth(),
	//	(float)m_BackMaterial->GetTextureHeight(), 1.f);

	return true;
}

void CTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CTileMapComponent::PostUpdate(float DeltaTime)
{
	CSceneComponent::PostUpdate(DeltaTime);
}

void CTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CTileMapComponent::Render()
{
	CSceneComponent::Render();

	if (m_BackMaterial)
	{
		m_BackMaterial->Render();

		m_BackMesh->Render();

		m_BackMaterial->Reset();
	}
}

void CTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CTileMapComponent* CTileMapComponent::Clone()
{
	return new CTileMapComponent(*this);
}

void CTileMapComponent::Save(FILE* File)
{
	std::string	MeshName = m_BackMesh->GetName();

	int	Length = (int)MeshName.length();

	fwrite(&Length, sizeof(int), 1, File);
	fwrite(MeshName.c_str(), sizeof(char), Length, File);

	m_BackMaterial->Save(File);

	CSceneComponent::Save(File);
}

void CTileMapComponent::Load(FILE* File)
{
	char	MeshName[256] = {};

	int	Length = 0;

	fread(&Length, sizeof(int), 1, File);
	fread(MeshName, sizeof(char), Length, File);

	m_BackMesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh(MeshName);

	m_BackMaterial = m_Scene->GetResource()->CreateMaterialEmpty<CMaterial>();

	m_BackMaterial->Load(File);

	CSceneComponent::Load(File);
}
