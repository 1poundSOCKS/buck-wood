#pragma once

class volume_controller
{

public:

  volume_controller() = default;
  template <std::size_t N> volume_controller(std::array<IDirectSoundBuffer8*, N> buffers);

  auto AddSoundBuffer(IDirectSoundBuffer8* buffer) -> void;
  auto SetVolume(float value) -> void;

private:

  using sound_buffer_ptr = winrt::com_ptr<IDirectSoundBuffer8>;
  using sound_buffer_collection = std::vector<sound_buffer_ptr>;
  sound_buffer_collection m_buffers;

};

template <std::size_t N> volume_controller::volume_controller(std::array<IDirectSoundBuffer8*, N> buffers)
{
  for( auto& buffer : buffers )
  {
    AddSoundBuffer(buffer);
  }
}

inline auto volume_controller::AddSoundBuffer(IDirectSoundBuffer8* buffer) -> void
{
  m_buffers.emplace_back( sound_buffer_ptr {} );
  m_buffers.back().attach(buffer);
  m_buffers.back()->AddRef();
}

inline auto volume_controller::SetVolume(float value) -> void
{
  auto volumeRange = float { DSBVOLUME_MAX - DSBVOLUME_MIN };
  auto volume = static_cast<int>(DSBVOLUME_MIN + volumeRange * value);

  for( auto& buffer : m_buffers )
  {
    buffer->SetVolume(volume);
  }
}
