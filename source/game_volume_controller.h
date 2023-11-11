#pragma once

#include "volume_controller.h"

class game_volume_controller
{

public:

  static auto create() -> void;

  static auto minVolume() -> int;
  static auto maxVolume() -> int;

  static auto setEffectsVolume(int value) -> int;
  static auto setMusicVolume(int value) -> int;

  static auto effectsVolume() -> int;
  static auto musicVolume() -> int;

private:

  static auto GetFractionalVolume(int value) -> float;

  game_volume_controller();

  auto SetEffectsVolume(int value) -> void;
  auto SetMusicVolume(int value) -> void;

private:

  static game_volume_controller* m_instance;

  volume_controller m_effectsVolumeController;
  volume_controller m_musicVolumeController;

};

inline auto game_volume_controller::create() -> void
{
  if( m_instance == nullptr )
  {
    m_instance = new game_volume_controller();
  }
}

inline auto game_volume_controller::GetFractionalVolume(int value) -> float
{
  auto volumeRange = maxVolume() - minVolume();
  return static_cast<float>(value) / volumeRange + minVolume();
}

inline auto game_volume_controller::SetEffectsVolume(int value) -> void
{
  m_effectsVolumeController.SetVolume(GetFractionalVolume(value));
}

inline auto game_volume_controller::SetMusicVolume(int value) -> void
{
  m_musicVolumeController.SetVolume(GetFractionalVolume(value));
}
