#pragma once

#include "GameInfo.h"

struct SkillInfo
{
    int Level;
    float CoolTime;
    float AccTime;
    float Factor;
    bool Active;
    int MPRequire;

    SkillInfo() :
        Level(0),
        CoolTime(0.f),
        AccTime(0.f),
        Factor(0.f),
        Active(false),
        MPRequire(0)
    {
    }
};

class CPlayerSkillInfo
{
	friend class CPlayer2D;
    friend class CSkillQuickSlotWindow;
    friend class CSkillPointWindow;

private:
	CPlayerSkillInfo();
	~CPlayerSkillInfo();

private:
    std::unordered_map<std::string, SkillInfo*>  m_mapSkillInfo;

private:
    virtual bool Init();
    virtual void Update(float DeltaTime);

public:
    SkillInfo*   FindSkillInfo(const std::string& Name);
};

