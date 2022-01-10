#include "SoundManager.h"

CSoundManager::CSoundManager() :
	m_System(nullptr),
	m_MasterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
	auto	iter = m_mapGroup.begin();
	auto	iterEnd = m_mapGroup.end();

	for (; iter != iterEnd; ++iter)
	{
		iter->second->release();
	}

	if (m_System)
	{
		m_System->close();
		m_System->release();
	}
}

bool CSoundManager::Init()
{
	FMOD_RESULT result = FMOD::System_Create(&m_System);

	if (result != FMOD_OK)
		return false;

	result = m_System->init(4093, FMOD_INIT_NORMAL, nullptr);

	if (result != FMOD_OK)
		return false;

	// MasterGroup을 얻어온다
	result = m_System->getMasterChannelGroup(&m_MasterGroup);

	if (result != FMOD_OK)
		return false;

	m_mapGroup.insert(std::make_pair("Master", m_MasterGroup));

	return true;
}


void CSoundManager::Update()
{
}
