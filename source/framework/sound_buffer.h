#ifndef _sound_buffer_
#define _sound_buffer_

#include "data_files.h"

using sound_buffers = std::vector<winrt::com_ptr<IDirectSoundBuffer8>>;

template <typename selector_name_type>
struct sound_buffer_selector
{
  const sound_buffers& buffers;
  IDirectSoundBuffer8* operator[](typename selector_name_type name)
  {
    return buffers[name].get();
  }
};

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file);

winrt::com_ptr<IDirectSoundBuffer8> CreateSoundBuffer(IDirectSound8* directSound, const wav_file_data& data);

#endif
