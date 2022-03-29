#pragma once

#include "GameInfo.h"

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

