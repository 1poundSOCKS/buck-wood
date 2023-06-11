#include "pch.h"
#include "sound_data.h"

constexpr [[nodiscard]] auto GetWavFilenames() -> std::array<LPCWSTR, 4>
{
  return   {
    L"main_theme.wav", 
    L"shoot_effect.wav", 
    L"thrust_effect.wav", 
    L"target_activated.wav"
  };
}

auto LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, std::back_insert_iterator<sound_buffers> soundBufferInserter) -> void
{
  auto wavFiles = GetWavFilenames();

  std::filesystem::path soundFilePath = path;
  soundFilePath /= L"sound";

  sound_buffers soundBuffers;
  std::transform(wavFiles.cbegin(), wavFiles.cend(), soundBufferInserter, [directSound, soundFilePath](const auto filename)
  {
    return LoadSoundBuffer(directSound, soundFilePath, filename);
  });
}

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
  LoadSoundBuffers(directSound, path, std::back_inserter(m_soundBuffers));
}
