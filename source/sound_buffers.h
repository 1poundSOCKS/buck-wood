#ifndef _sound_buffers_
#define _sound_buffers_

#include <filesystem>
#include <memory>
#include "sound_buffer.h"

namespace fs = std::filesystem;

struct sound_buffers
{
  sound_buffers(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path);

  sound_buffer_ptr themeTune;
  sound_buffer_ptr shootEffect;
  sound_buffer_ptr thrustEffect;
};

using sound_buffers_ptr = std::unique_ptr<sound_buffers>;

sound_buffer_ptr InitializeSoundBuffer(const winrt::com_ptr<IDirectSound8>& directSound, const std::wstring& path, const std::wstring& file);

#endif
