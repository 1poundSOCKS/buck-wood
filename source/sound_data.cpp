#include "pch.h"
#include "sound_data.h"

sound_data* sound_data::m_soundData = nullptr;

auto sound_data::create(IDirectSound8* directSound, const std::wstring& path) -> void
{
  m_soundData = new sound_data(directSound, path);
}

auto sound_data::soundBuffers() -> sound_buffers&
{
  return m_soundData->m_soundBuffers;
}

sound_data::sound_data(IDirectSound8* directSound, const std::wstring& path)
{
  m_soundBuffers.Load(directSound, path, GetWavFilenames());
  // LoadSoundBuffers(directSound, path, std::back_inserter(m_soundBuffers));
}
