
#include "PlayerSkillInfo.h"

CPlayerSkillInfo::CPlayerSkillInfo()
{
    SkillInfo* SylphideLancer = new SkillInfo;
    SylphideLancer->Level = 20;
    SylphideLancer->CoolTime = 1.8f;
    SylphideLancer->Factor = 1.f;

    m_mapSkillInfo.insert(std::make_pair("SylphideLancer", SylphideLancer));


    SkillInfo* VoidPressure = new SkillInfo;
    VoidPressure->Level = 20;
    VoidPressure->CoolTime = 1.8f;
    VoidPressure->Factor = 2.f;

    m_mapSkillInfo.insert(std::make_pair("VoidPressure", VoidPressure));


    SkillInfo* LightTransforming = new SkillInfo;
    LightTransforming->Level = 20;
    LightTransforming->CoolTime = 1.1f;

    m_mapSkillInfo.insert(std::make_pair("LightTransforming", LightTransforming));


    SkillInfo* NoxSpear = new SkillInfo;
    NoxSpear->Level = 20;
    NoxSpear->CoolTime = 2.f;
    NoxSpear->Factor = 1.5f;

    m_mapSkillInfo.insert(std::make_pair("NoxSpear", NoxSpear));


    SkillInfo* DeathSide = new SkillInfo;
    DeathSide->Level = 3;
    DeathSide->CoolTime = 8.f;
    DeathSide->Factor = 1.5f;

    m_mapSkillInfo.insert(std::make_pair("DeathSide", DeathSide));
}

CPlayerSkillInfo::~CPlayerSkillInfo()
{
    auto iter = m_mapSkillInfo.begin();
    auto iterEnd = m_mapSkillInfo.end();

    for (; iter != iterEnd; ++iter)
    {
        SAFE_DELETE(iter->second);
    }
}

bool CPlayerSkillInfo::Init()
{
	return true;
}

void CPlayerSkillInfo::Update(float DeltaTime)
{
    auto iter = m_mapSkillInfo.begin();
    auto iterEnd = m_mapSkillInfo.end();

    for (; iter != iterEnd; ++iter)
    {
        if (iter->second->Active == true)
        {
            iter->second->AccTime += DeltaTime;
            if (iter->second->AccTime >= iter->second->CoolTime)
            {
                iter->second->Active = false;
                iter->second->AccTime = 0.f;
            }
        }
    }
}

SkillInfo* CPlayerSkillInfo::FindSkillInfo(const std::string& Name)
{
   auto iter = m_mapSkillInfo.find(Name);
   auto iterEnd = m_mapSkillInfo.end();

   if (iter == iterEnd)
       return nullptr;

   return iter->second;
}
