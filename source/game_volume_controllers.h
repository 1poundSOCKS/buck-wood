#pragma once

#include "volume_controller.h"

class game_volume_controllers
{

public:

  static auto create() -> void;

  static auto setEffectsVolume(int value) -> int;
  static auto setMusicVolume(int value) -> int;

  static auto effectsVolume() -> int;
  static auto musicVolume() -> int;

private:

  static auto GetFractionalVolume(int value) -> float;

  game_volume_controllers();

  auto SetEffectsVolume(int value) -> void;
  auto SetMusicVolume(int value) -> void;

private:

  static game_volume_controllers* m_instance;

  volume_controller m_effectsVolumeController;
  volume_controller m_musicVolumeController;

};

inline auto game_volume_controllers::create() -> void
{
  if( m_instance == nullptr )
  {
    m_instance = new game_volume_controllers();
  }
}

inline auto game_volume_controllers::GetFractionalVolume(int value) -> float
{
  return static_cast<float>(value) / 10;
}

inline auto game_volume_controllers::SetEffectsVolume(int value) -> void
{
  m_effectsVolumeController.SetVolume(GetFractionalVolume(value));
}

inline auto game_volume_controllers::SetMusicVolume(int value) -> void
{
  m_musicVolumeController.SetVolume(GetFractionalVolume(value));
}
