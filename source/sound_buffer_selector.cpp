#include "pch.h"
#include "sound_buffer_selector.h"

// sound_buffers::sound_buffers(IDirectSound8* directSound, const std::wstring& path)
// {
//   menuTheme = LoadSoundBuffer(directSound, path, L"main_theme.wav");
//   shoot = LoadSoundBuffer(directSound, path, L"shoot_effect.wav");
//   thrust = LoadSoundBuffer(directSound, path, L"thrust_effect.wav");
//   targetActivated = LoadSoundBuffer(directSound, path, L"target_activated.wav");
// }

void LoadSoundBuffers(IDirectSound8* directSound, const std::wstring& path, sound_buffers& soundBuffers)
{
  fs::path soundFilePath = path;
  soundFilePath /= L"sound";

  soundBuffers.clear();
  soundBuffers.emplace_back(LoadSoundBuffer(directSound, soundFilePath, L"main_theme.wav"));
  soundBuffers.emplace_back(LoadSoundBuffer(directSound, soundFilePath, L"shoot_effect.wav"));
  soundBuffers.emplace_back(LoadSoundBuffer(directSound, soundFilePath, L"thrust_effect.wav"));
  soundBuffers.emplace_back(LoadSoundBuffer(directSound, soundFilePath, L"target_activated.wav"));
}
