#ifndef _sound_buffers_
#define _sound_buffers_

#include <filesystem>
#include <memory>
#include "framework/framework.h"

namespace fs = std::filesystem;

struct sound_buffers
{
  sound_buffers(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path);

  std::unique_ptr<sound_buffer> menuTheme;
  std::unique_ptr<sound_buffer> shoot;
  std::unique_ptr<sound_buffer> thrust;
  std::unique_ptr<sound_buffer> targetActivated;
};

using sound_buffers_ptr = std::unique_ptr<sound_buffers>;

std::unique_ptr<sound_buffer> LoadSoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path, const std::wstring& file);

#endif
