#pragma once

#include "Sound.h"

class CSoundManager
{
	friend class CResourceManager;

private:
	CSoundManager();
	~CSoundManager();

private:
	FMOD::System* m_System;
	FMOD::ChannelGroup* m_MasterGroup;
	std::unordered_map<std::string, FMOD::ChannelGroup*>	m_mapGroup;
	std::unordered_map<std::string, CSharedPtr<CSound>>		m_mapSound;

public:
	bool Init();
	void Update();
};

