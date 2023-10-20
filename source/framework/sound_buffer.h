#ifndef _sound_buffer_
#define _sound_buffer_

#include "sound_buffers.h"
#include "data_files.h"

template <typename selector_name_type>
struct sound_buffer_selector
{
  const sound_buffers& buffers;
  IDirectSoundBuffer8* operator[](selector_name_type name) const
  {
    return buffers[name].get();
  }
};

class sound_buffer
{

public:

  sound_buffer() = default;
  sound_buffer(IDirectSoundBuffer8* buffer);

private:

  winrt::com_ptr<IDirectSoundBuffer8> m_buffer;

};

inline sound_buffer::sound_buffer(IDirectSoundBuffer8* buffer)
{
  if( buffer )
  {
    m_buffer.attach(buffer);
    m_buffer->AddRef();
  }
}

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file);

winrt::com_ptr<IDirectSoundBuffer8> CreateSoundBuffer(IDirectSound8* directSound, const wav_file_data& data);

#endif
