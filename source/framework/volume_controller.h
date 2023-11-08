#pragma once

#include "sound_buffer.h"

class volume_controller
{

public:

  volume_controller() = default;
  template <std::size_t N> volume_controller(std::array<sound_buffer, N> buffers);

  auto AddSoundBuffer(const sound_buffer& soundBuffer) -> void;
  auto SetVolume(float value) -> void;
  [[nodiscard]] auto Volume() const -> float;

private:

  using sound_buffer_collection = std::vector<sound_buffer>;
  sound_buffer_collection m_buffers;
  float m_volume { 1.0 };

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
  m_volume = value;

  for( auto& buffer : m_buffers )
  {
    buffer.SetVolume(value);
  }
}

[[nodiscard]] inline auto volume_controller::Volume() const -> float
{
  return m_volume;
}
