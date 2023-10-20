#ifndef _sound_buffer_
#define _sound_buffer_

#include "data_files.h"

// template <typename selector_name_type>
// struct sound_buffer_selector
// {
//   const sound_buffers& buffers;
//   IDirectSoundBuffer8* operator[](selector_name_type name) const
//   {
//     return buffers[name].get();
//   }
// };

winrt::com_ptr<IDirectSoundBuffer8> LoadSoundBuffer(IDirectSound8* directSound, const std::wstring& path, const std::wstring& file);
winrt::com_ptr<IDirectSoundBuffer8> CreateSoundBuffer(IDirectSound8* directSound, const wav_file_data& data);
void PlaySoundBuffer(IDirectSoundBuffer8* soundBuffer, bool loop=false);
void StopSoundBufferPlay(IDirectSoundBuffer8* soundBuffer);
void ResetSoundBuffer(IDirectSoundBuffer8* soundBuffer);

class sound_buffer
{

public:

  sound_buffer() = default;
  sound_buffer(IDirectSoundBuffer8* buffer);
  sound_buffer(const winrt::com_ptr<IDirectSoundBuffer8>& buffer);

  [[nodiscard]] operator bool() const;

  auto SetVolume(float value) const -> void;
  auto Play(bool loop) const -> void;
  auto Stop() const -> void;
  auto Reset() const -> void;

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

 inline sound_buffer::sound_buffer(const winrt::com_ptr<IDirectSoundBuffer8>& buffer) : m_buffer { buffer }
{
}

[[nodiscard]] inline sound_buffer::operator bool() const
{
  return static_cast<bool>(m_buffer);
}

inline auto sound_buffer::Play(bool loop) const -> void
{
  if( m_buffer )
  {
    PlaySoundBuffer(m_buffer.get(), loop);
  }
}

inline auto sound_buffer::Stop() const -> void
{
  if( m_buffer )
  {
    StopSoundBufferPlay(m_buffer.get());
  }
}

inline auto sound_buffer::Reset() const -> void
{
  if( m_buffer )
  {
    ResetSoundBuffer(m_buffer.get());
  }
}

inline auto sound_buffer::SetVolume(float value) const -> void
{
  if( m_buffer )
  {
    auto volumeRange = float { DSBVOLUME_MAX - DSBVOLUME_MIN };
    auto volume = static_cast<int>(DSBVOLUME_MIN + volumeRange * value);
    m_buffer->SetVolume(volume);
  }
}

#endif
