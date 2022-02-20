#pragma once

#include "GameObject/GameObject.h"

class CVoidPressureOrb :
    public CGameObject
{
public:
	CVoidPressureOrb();
	CVoidPressureOrb(const CVoidPressureOrb& Anim);
	~CVoidPressureOrb();

private:
	CSharedPtr<class CSpriteComponent>      m_Sprite;
	Vector3                                 m_OriginPos;

public:
    void SetOriginPos(const Vector3& Pos)
    {
        m_OriginPos = Pos;
    }

    void SetOriginPos(float X, float Y, float Z)
    {
        m_OriginPos = Vector3(X, Y, Z);
    }

    Vector3 GetOriginPos()  const
    {
        return m_OriginPos;
    }


public:
    virtual void FlipAll(float DeltaTime);
	virtual bool Init();
	virtual void Update(float DeltaTime);
	virtual void PostUpdate(float DeltaTime);
	virtual CVoidPressureOrb* Clone();

private:
	void EndSkillEffect();
};

