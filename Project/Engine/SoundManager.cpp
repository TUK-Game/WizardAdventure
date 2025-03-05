#include "pch.h"
#include "SoundManager.h"
#include "Sound.h"

CSoundManager::CSoundManager() :
    m_system(nullptr),
    m_masterGroup(nullptr)
{
}

CSoundManager::~CSoundManager()
{
    m_mapSound.clear();

    for (auto& [name, group] : m_mapChannelGroup)
    {
        group->release();
    }
  
    if (m_system)
    {
        m_system->release();
        m_system->close();
    }
}

bool CSoundManager::Init()
{
    FMOD_RESULT result = FMOD::System_Create(&m_system);

    if (result != FMOD_OK)
        return false;

    result = m_system->init(500, FMOD_INIT_NORMAL, nullptr);

    if (result != FMOD_OK)
        return false;

    result = m_system->getMasterChannelGroup(&m_masterGroup);

    if (result != FMOD_OK)
        return false;

    m_mapChannelGroup.insert(std::make_pair("Master", m_masterGroup));

    CreateSoundChannel("BGM");
    CreateSoundChannel("Effect");
    CreateSoundChannel("UI");

    return true;
}

void CSoundManager::Update()
{
    m_system->update();
}

bool CSoundManager::CreateSoundChannel(const std::string& name)
{
    if (FindChannelGroup(name))
        return false;

    FMOD::ChannelGroup* group = nullptr;

    FMOD_RESULT	result = m_system->createChannelGroup(name.c_str(), &group);

    if (result != FMOD_OK)
        return false;

    // 생성한 그룹을 마스터 그룹에 추가
    m_masterGroup->addGroup(group, false);

    m_mapChannelGroup.insert(std::make_pair(name, group));

    return true;
}

bool CSoundManager::LoadSound(const std::string& groupName, const std::string& name, bool loop, const char* fileName, const std::wstring& pathName)
{
    CSound* sound = FindSound(name);

    if (sound)
        return true;

    FMOD::ChannelGroup* group = FindChannelGroup(groupName);

    if (!group)
        return false;

    sound = new CSound;
    std::wstring wname = s2ws(name); 
    sound->SetName(wname);

    if (!sound->LoadSound(m_system, group, loop, fileName, pathName))
    {
        //SAFE_DELETE(sound);
        if (sound) {
            delete sound;
            sound = nullptr;
        }
        return false;
    }

    m_mapSound.insert(std::make_pair(name, sound));

    return true;
}

bool CSoundManager::SetVolume(int volume)
{
    m_masterGroup->setVolume(volume / 100.f);

    return true;
}

bool CSoundManager::SetVolume(const std::string& groupName, int volume)
{
    FMOD::ChannelGroup* group = FindChannelGroup(groupName);

    if (!group)
        return false;

    group->setVolume(volume / 100.f);

    return true;
}

bool CSoundManager::SoundPlay(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Play();

    return true;
}

bool CSoundManager::SoundStop(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Stop();

    return true;
}

bool CSoundManager::SoundPause(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Pause();

    return true;
}

bool CSoundManager::SoundResume(const std::string& name)
{
    CSound* sound = FindSound(name);

    if (!sound)
        return false;

    sound->Resume();

    return true;
}

FMOD::ChannelGroup* CSoundManager::FindChannelGroup(const std::string& name)
{
    auto iter = m_mapChannelGroup.find(name);

    if (iter == m_mapChannelGroup.end())
        return nullptr;

    return iter->second;
}

CSound* CSoundManager::FindSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter == m_mapSound.end())
        return nullptr;

    return iter->second;
}

void CSoundManager::ReleaseSound(const std::string& name)
{
    auto iter = m_mapSound.find(name);

    if (iter == m_mapSound.end())
        return;

    if (iter->second->GetRefCount() == 1)
        m_mapSound.erase(iter);
}
