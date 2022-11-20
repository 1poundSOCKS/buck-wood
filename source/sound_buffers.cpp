#include "pch.h"
#include "sound_buffers.h"

sound_buffers::sound_buffers(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path)
{
  menuTheme = LoadSoundBuffer(directSound, path, L"main_theme.wav");
  shoot = LoadSoundBuffer(directSound, path, L"shoot_effect.wav");
  thrust = LoadSoundBuffer(directSound, path, L"thrust_effect.wav");
  targetActivated = LoadSoundBuffer(directSound, path, L"target_activated.wav");
}

std::unique_ptr<sound_buffer> LoadSoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path, const std::wstring& file)
{
  fs::path filename = path;
  filename /= L"sound";
  filename /= file;
  wav_file_data_ptr soundData = std::make_unique<wav_file_data>(filename.c_str());
  return std::make_unique<sound_buffer>(directSound, *soundData);
}
