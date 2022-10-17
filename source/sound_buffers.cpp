#include "sound_buffers.h"

sound_buffers::sound_buffers(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path)
{
  themeTune = InitializeSoundBuffer(directSound, path, L"main_theme.wav");
  shootEffect = InitializeSoundBuffer(directSound, path, L"shoot_effect.wav");
  thrustEffect = InitializeSoundBuffer(directSound, path, L"thrust_effect.wav");
}

sound_buffer_ptr InitializeSoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path, const std::wstring& file)
{
  fs::path filename = path;
  filename /= L"sound";
  filename /= file;
  wav_file_data_ptr soundData = std::make_unique<wav_file_data>(filename.c_str());
  return std::make_unique<sound_buffer>(directSound, *soundData);
}
