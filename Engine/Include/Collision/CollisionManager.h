#pragma once

#include "../GameInfo.h"

class CCollisionManager
{
private:
	std::unordered_map<std::string, CollisionProfile*>	m_mapProfile;

public:
	bool Init();
	bool CreateProfile(const std::string& Name, Collision_Channel Channel, bool Enable,
		Collision_State State = Collision_State::Collision);
	bool SetCollisionState(const std::string& Name, Collision_Channel Channel,
		Collision_State State);
	CollisionProfile* FindProfile(const std::string& Name);

	DECLARE_SINGLE(CCollisionManager)
};

