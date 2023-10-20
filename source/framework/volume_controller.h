#pragma once

#include "sound_buffer.h"

class volume_controller
{

public:

  volume_controller() = default;
  template <std::size_t N> volume_controller(std::array<sound_buffer, N> buffers);

  auto AddSoundBuffer(const sound_buffer& soundBuffer) -> void;
  auto SetVolume(float value) -> void;

private:

  using sound_buffer_collection = std::vector<sound_buffer>;
  sound_buffer_collection m_buffers;

};

template <std::size_t N> volume_controller::volume_controller(std::array<sound_buffer, N> buffers)
{
  for( auto& buffer : buffers )
  {
    AddSoundBuffer(buffer);
  }
}

inline auto volume_controller::AddSoundBuffer(const sound_buffer& soundBuffer) -> void
{
  if( soundBuffer )
  {
    m_buffers.push_back(soundBuffer);
  }
}

inline auto volume_controller::SetVolume(float value) -> void
{
  for( auto& buffer : m_buffers )
  {
    buffer.SetVolume(value);
  }
}
