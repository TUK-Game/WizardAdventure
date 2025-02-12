#include "pch.h"
#include "Sound.h"
#include "PathManager.h"

CSound::CSound() :
	m_system(nullptr),
	m_sound(nullptr),
	m_group(nullptr),
	m_channel(nullptr),
	m_bPlay(false),
	m_bLoop(false),
	m_bPause(false)
{
}

CSound::~CSound()
{
	if (m_sound)
	{
		m_sound->release();
	}
}

bool CSound::LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const char* fileName, const std::wstring& pathName)
{
	m_system = system;
	m_group = group;
	m_bLoop = loop;

	std::filesystem::path path = CPathManager::GetInst()->FindPath(pathName);
	path /= fileName;

	FMOD_MODE mode = FMOD_DEFAULT;

	if (m_bLoop)
		mode = FMOD_LOOP_NORMAL;

	if (m_system->createSound(path.string().c_str(), mode, nullptr, &m_sound) != FMOD_OK)
		return false;

	return true;
}

void CSound::Play()
{
	m_system->playSound(m_sound, m_group, false, &m_channel);
	m_bPlay = true;
	m_bPause = false;
}

void CSound::Stop()
{
	if (m_channel)
	{
		bool bPlaying = false;

		m_channel->isPlaying(&bPlaying);

		if (bPlaying)
		{
			m_channel->stop();
			m_channel = nullptr;

			m_bPlay = false;
			m_bPause = false;
		}
	}
}

void CSound::Pause()
{
	if (m_channel)
	{
		bool bPlaying = false;

		m_channel->isPlaying(&bPlaying);

		if (bPlaying)
			m_channel->setPaused(true);

		m_bPlay = false;
		m_bPause = true;
	}
}

void CSound::Resume()
{
	if (m_channel)
	{
		if (m_bPause)
		{
			m_channel->setPaused(false);

			m_bPlay = true;
			m_bPause = false;
		}
	}
}
