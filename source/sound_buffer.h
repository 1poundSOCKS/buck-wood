#ifndef _sound_buffer_
#define _sound_buffer_

#include <winrt/base.h>
#include <dsound.h>
#include "data_files.h"

struct sound_buffer
{
  sound_buffer(const winrt::com_ptr<IDirectSound8>& directSound, const wav_file_data& data);

  winrt::com_ptr<IDirectSoundBuffer8> buffer;
};

using sound_buffer_ptr = std::unique_ptr<sound_buffer>;

#endif
