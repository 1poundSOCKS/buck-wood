#include "pch.h"
#include "sound_buffers.h"

sound_buffers::sound_buffers(IDirectSound8* directSound, const std::wstring& path)
{
  menuTheme = LoadSoundBuffer(directSound, path, L"main_theme.wav");
  shoot = LoadSoundBuffer(directSound, path, L"shoot_effect.wav");
  thrust = LoadSoundBuffer(directSound, path, L"thrust_effect.wav");
  targetActivated = LoadSoundBuffer(directSound, path, L"target_activated.wav");
}

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file)
{
  fs::path filename = path;
  filename /= L"sound";
  filename /= file;
  wav_file_data_ptr soundData = std::make_unique<wav_file_data>(filename.c_str());
  return CreateSoundBuffer(directSound, *soundData);
}
