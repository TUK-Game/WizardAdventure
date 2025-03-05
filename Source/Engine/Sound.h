#pragma once

#include "Asset.h"

class CSound :
    public CRef
{
    friend class CSoundManager;

private:
    CSound();
    ~CSound();

public:
    bool GetPlay() const    { return m_bPlay; }
    bool GetLoop() const    { return m_bLoop; }

public:
    bool LoadSound(FMOD::System* system, FMOD::ChannelGroup* group, bool loop, const char* fileName, const std::wstring& pathName = SOUND_PATH);
    void Play();
    void Stop();
    void Pause();
    void Resume();
    virtual CSound* Clone() override { return new CSound(*this); }

private:
    FMOD::System*       m_system;
    FMOD::Sound*        m_sound;
    FMOD::ChannelGroup* m_group;
    FMOD::Channel*      m_channel;
    bool        m_bPlay;
    bool        m_bLoop;
    bool        m_bPause;
};
