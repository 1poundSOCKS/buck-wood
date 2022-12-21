
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

[[nodiscard]] auto LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path) -> sound_buffers
{
  auto wavFiles = GetWavFilenames();

  fs::path soundFilePath = path;
  soundFilePath /= L"sound";

  sound_buffers soundBuffers;
  std::transform(wavFiles.cbegin(), wavFiles.cend(), std::back_inserter(soundBuffers), [directSound, soundFilePath](const auto filename)
  {
    return LoadSoundBuffer(directSound, soundFilePath, filename);
  });

  return soundBuffers;
}
