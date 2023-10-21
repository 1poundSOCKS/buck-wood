#ifndef _sound_buffer_
#define _sound_buffer_

#include "data_files.h"

class sound_buffer
{

public:

  sound_buffer() = default;
  sound_buffer(IDirectSound8* directSound, const wav_file_data& data);
  sound_buffer(IDirectSoundBuffer8* buffer);
  sound_buffer(const winrt::com_ptr<IDirectSoundBuffer8>& buffer);

  auto Create(IDirectSound8* directSound, const wav_file_data& data) -> void;

  [[nodiscard]] operator bool() const;

  auto SetVolume(float value) const -> void;
  auto Play(bool loop) const -> void;
  auto Stop() const -> void;
  auto Reset() const -> void;

private:

  winrt::com_ptr<IDirectSoundBuffer8> m_buffer;

};

inline sound_buffer::sound_buffer(IDirectSound8* directSound, const wav_file_data& data)
{
  Create(directSound, data);
}

inline sound_buffer::sound_buffer(IDirectSoundBuffer8* buffer)
{
  if( buffer )
  {
    m_buffer.attach(buffer);
    m_buffer->AddRef();
  }
}

inline sound_buffer::sound_buffer(const winrt::com_ptr<IDirectSoundBuffer8>& buffer) : m_buffer { buffer }
{
}

[[nodiscard]] inline sound_buffer::operator bool() const
{
  return static_cast<bool>(m_buffer);
}

#endif
