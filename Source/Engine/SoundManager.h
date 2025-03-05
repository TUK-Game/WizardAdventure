#pragma once

class CSoundManager
{
	friend class CAssetManager;

private:
	CSoundManager();
	~CSoundManager();

public:
	bool Init();
	void Update();
	bool CreateSoundChannel(const std::string& name);
	bool LoadSound(const std::string& groupName, const std::string& name, bool loop, const char* fileName, const std::wstring& pathName = SOUND_PATH);
	bool SetVolume(int volume);
	bool SetVolume(const std::string& groupName, int volume);
	bool SoundPlay(const std::string& name);
	bool SoundStop(const std::string& name);
	bool SoundPause(const std::string& name);
	bool SoundResume(const std::string& name);

	FMOD::ChannelGroup* FindChannelGroup(const std::string& name);
	class CSound* FindSound(const std::string& name);
	void ReleaseSound(const std::string& name);

private:
	std::unordered_map<std::string, CSharedPtr<class CSound>>	m_mapSound;
	std::unordered_map<std::string, FMOD::ChannelGroup*>		m_mapChannelGroup;
	FMOD::System*		m_system;
	FMOD::ChannelGroup* m_masterGroup;
};

