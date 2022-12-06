
#include "pch.h"
#include "screen_sound_data.h"

void LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, sound_buffers& soundBuffers)
{
  static const std::array<LPCWSTR, 4> wavFiles
  {
    L"main_theme.wav", 
    L"shoot_effect.wav", 
    L"thrust_effect.wav", 
    L"target_activated.wav"
  };

  fs::path soundFilePath = path;
  soundFilePath /= L"sound";

  soundBuffers.clear();
  std::transform(wavFiles.cbegin(), wavFiles.cend(), std::back_inserter(soundBuffers), [directSound, soundFilePath](const auto filename)
  {
    return LoadSoundBuffer(directSound, soundFilePath, filename);
  });
}
