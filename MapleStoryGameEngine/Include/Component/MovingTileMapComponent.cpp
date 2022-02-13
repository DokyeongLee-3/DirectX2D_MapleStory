
#include "MovingTileMapComponent.h"
#include "../Scene/Scene.h"
#include "../Scene/SceneResource.h"
#include "../Resource/Shader/MovingTileMapConstantBuffer.h"
#include "../Resource/Shader/StructuredBuffer.h"
#include "CameraComponent.h"
#include "../Scene/CameraManager.h"

CMovingTileMapComponent::CMovingTileMapComponent()
{
	SetTypeID<CMovingTileMapComponent>();
	m_Count = 0;
	m_Render = true;
	m_IsRepeat = true;
}

CMovingTileMapComponent::CMovingTileMapComponent(const CMovingTileMapComponent& com)	:
	CSceneComponent(com)
{
	m_Mesh = com.m_Mesh;

	if (com.m_Material)
		m_Material = com.m_Material->Clone();

	if (com.m_CBuffer)
		m_CBuffer = com.m_CBuffer->Clone();

	if (com.m_MovingTileInfo)
		m_MovingTileInfo = com.m_MovingTileInfo->Clone();

}

CMovingTileMapComponent::~CMovingTileMapComponent()
{
	SAFE_DELETE(m_MovingTileInfo);
	SAFE_DELETE(m_CBuffer);

	size_t	Size = m_vecMovingTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		SAFE_DELETE(m_vecMovingTile[i]);
	}

	m_vecMovingTile.clear();
}

void CMovingTileMapComponent::CreateMovingTile(int Count, const Vector2& Size)
{
	m_Count = Count;
	
	for (int i = 1; i <= m_Count; ++i)
	{
		// MovingTile  ����
		CMovingTile* MovingTile = new CMovingTile;

		// 2��°���� �����Ǵ� MovingTile�� ũ�⸸ŭ �̵����Ѽ� ��ġ�� �ʰ� ���ش�
		MovingTile->SetPos((i - 1) * Size.x, 35.f);
		MovingTile->SetOpacity(1.f);
		MovingTile->SetSize(Size.x, Size.y);
		MovingTile->SetSpeed(m_Speed);
		MovingTile->SetOwner(this);

		m_vecMovingTile.push_back(MovingTile);
	}

	CreateStructuredBuffer();
}

void CMovingTileMapComponent::CreateStructuredBuffer()
{
	SAFE_DELETE(m_MovingTileInfo);

	m_MovingTileInfo = new CStructuredBuffer;

	// C++�ڵ忡�� ����ȭ���� ������ ä���־�� �ϹǷ� Dynamic�� true��
	m_MovingTileInfo->Init("MovingTileInfo", sizeof(MovingTileInfo), m_Count, 50, true, (int)Buffer_Shader_Type::Vertex);

	m_vecMovingTileInfo.resize(m_Count);

	for (int i = 0; i < m_Count; ++i)
	{
		m_vecMovingTileInfo[i].Opacity = 1.f;
		m_vecMovingTileInfo[i].Start = m_vecMovingTile[i]->GetFrameStart();
		m_vecMovingTileInfo[i].End = m_vecMovingTile[i]->GetFrameEnd();
		m_vecMovingTileInfo[i].Opacity = m_vecMovingTile[i]->GetOpacity();
	}
}

void CMovingTileMapComponent::SetRepeatPos(const Vector2& Pos)
{
	m_RepeatPos = Pos;
}

void CMovingTileMapComponent::SetMaterial(CMaterial* Material)
{
	m_Material = Material->Clone();

	m_CBuffer->SetImageSize(Vector2((float)m_Material->GetTextureWidth(), (float)m_Material->GetTextureHeight()));
}

void CMovingTileMapComponent::SetDefaultFrame(const Vector2& Start, const Vector2& End)
{
	size_t Size = m_vecMovingTileInfo.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecMovingTile[i]->SetFrameStart(Start);
		m_vecMovingTile[i]->SetFrameEnd(End);
	}
}

void CMovingTileMapComponent::SetDefaultFrame(float StartX, float StartY, float EndX, float EndY)
{
	size_t Size = m_vecMovingTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecMovingTile[i]->SetFrameStart(StartX, StartY);
		m_vecMovingTile[i]->SetFrameEnd(EndX, EndY);
	}
}

void CMovingTileMapComponent::SetFrame(int Index, const Vector2& Start, const Vector2& End)
{
	if (Index >= m_Count)
		return;

	m_vecMovingTile[Index]->SetFrameStart(Start);
	m_vecMovingTile[Index]->SetFrameEnd(End);
	//m_vecMovingTile[Index]->SetSize(Vector2(End - Start));
}

void CMovingTileMapComponent::SetFrame(int Index, float StartX, float StartY, float EndX, float EndY)
{
	if (Index >= m_Count)
		return;

	m_vecMovingTile[Index]->SetFrameStart(StartX, StartY);
	m_vecMovingTile[Index]->SetFrameEnd(EndX, EndY);
	//m_vecMovingTile[Index]->SetSize(Vector2(EndX - StartX, EndY - StartY));
}

void CMovingTileMapComponent::SetOpacity(int Index, float Opacity)
{
	if (Index >= m_Count)
		return;

	m_vecMovingTile[Index]->SetOpacity(Opacity);
}

void CMovingTileMapComponent::SetCount(int Count)
{
	m_Count = Count;
}

void CMovingTileMapComponent::SetSpeed(const Vector2& Speed)
{
	m_Speed = Speed;
}

void CMovingTileMapComponent::Start()
{
}

bool CMovingTileMapComponent::Init()
{
	m_Mesh = (CSpriteMesh*)m_Scene->GetResource()->FindMesh("SpriteMesh");
	//SetMaterial(m_Scene->GetResource()->FindMaterial("BaseTexture"));

	SetMeshSize(1.f, 1.f, 0.f);
	//SetWorldScale((float)m_BackMaterial->GetTextureWidth(),
	//	(float)m_BackMaterial->GetTextureHeight(), 1.f);

	m_CBuffer = new CMovingTileMapConstantBuffer;

	m_CBuffer->Init();

	return true;
}

void CMovingTileMapComponent::Update(float DeltaTime)
{
	CSceneComponent::Update(DeltaTime);
}

void CMovingTileMapComponent::PostUpdate(float DeltaTime)
{
	size_t Size = m_vecMovingTile.size();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecMovingTile[i]->Update(DeltaTime);
	}

	CCameraComponent* Camera = m_Scene->GetCameraManager()->GetCurrentCamera();
	Matrix	matView, matProj;
	matView = Camera->GetViewMatrix();
	matProj = Camera->GetProjMatrix();

	for (size_t i = 0; i < Size; ++i)
	{
		m_vecMovingTileInfo[i].Start = m_vecMovingTile[i]->GetFrameStart();
		m_vecMovingTileInfo[i].End = m_vecMovingTile[i]->GetFrameEnd();
		m_vecMovingTileInfo[i].Opacity = m_vecMovingTile[i]->GetOpacity();
		// �̹� �� Ÿ���� World����� CTile::Update���� ���� ���� �����Ƿ� ���� World����� �ε����� �´� ����ȭ���۷� �ѱ��
		m_vecMovingTileInfo[i].matWVP = m_vecMovingTile[i]->GetWorldMatrix() * matView * matProj;
		m_vecMovingTileInfo[i].matWVP.Transpose();
	}

	m_MovingTileInfo->UpdateBuffer(&m_vecMovingTileInfo[0], m_Count);
}

void CMovingTileMapComponent::PrevRender()
{
	CSceneComponent::PrevRender();
}

void CMovingTileMapComponent::Render()
{
	CSceneComponent::Render();

	if (m_Material)
	{
		m_MovingTileInfo->SetShader();

		m_CBuffer->UpdateCBuffer();

		m_Material->Render();

		m_Mesh->RenderInstancing(m_Count);

		m_Material->Reset();

		m_MovingTileInfo->ResetShader();
	}
}

void CMovingTileMapComponent::PostRender()
{
	CSceneComponent::PostRender();
}

CMovingTileMapComponent* CMovingTileMapComponent::Clone()
{
	return new CMovingTileMapComponent(*this);
}

void CMovingTileMapComponent::Save(FILE* File)
{
}

void CMovingTileMapComponent::Load(FILE* File)
{
}
