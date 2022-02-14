
#include "MovingTile.h"
#include "MovingTileMapComponent.h"

CMovingTile::CMovingTile()	:
	m_Opacity(1.f),
	m_Owner(nullptr)
{
}

CMovingTile::CMovingTile(const CMovingTile& tile)
{
}

CMovingTile::~CMovingTile()
{
}

void CMovingTile::SetOwner(CMovingTileMapComponent* Owner)
{
	m_Owner = Owner;
}

void CMovingTile::Start()
{
}

void CMovingTile::Update(float DeltaTime)
{

	if (m_Owner)
	{
		if (m_Owner->IsRepeat())
		{
			if (m_Pos.x + m_Size.x <= 0.f)
			{
				Vector2 RepeatPos = m_Owner->GetRepeatPos();

				m_Pos.x = RepeatPos.x;
			}

			else
				m_Pos += m_Speed * DeltaTime;
		}
	}


	Matrix	matScale, matTranslate;

	matScale.Scaling(m_Size.x, m_Size.y, 1.f);
	matTranslate.Translation(m_Pos);

	m_matWorld = matScale * matTranslate;
}
