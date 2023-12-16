#pragma once

#include "framework.h"
#include "volume_controller.h"

class game_volume_controller
{

public:

  static auto create() -> void;
  static auto destroy() -> void;

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

  inline static game_volume_controller* m_instance { nullptr };
  volume_controller m_effectsVolumeController;
  volume_controller m_musicVolumeController;

};

inline auto game_volume_controller::create() -> void
{
  log::write(log::type::info, "creating game volume controller");
  destroy();
  m_instance = new game_volume_controller();
}

inline auto game_volume_controller::destroy() -> void
{
  if( m_instance )
  {
    log::write(log::type::info, "destroying game volume controller");
    delete m_instance;
    m_instance = nullptr;
  }
}

inline auto game_volume_controller::GetFractionalVolume(int value) -> float
{
  auto volumeRange = maxVolume() - minVolume();
  return static_cast<float>(value) / volumeRange + minVolume();
}

inline auto game_volume_controller::SetEffectsVolume(int value) -> void
{
  log::write(log::type::info, "setting effects volume to {}", value);
  m_effectsVolumeController.SetVolume(GetFractionalVolume(value));
}

inline auto game_volume_controller::SetMusicVolume(int value) -> void
{
  log::write(log::type::info, "setting music volume to {}", value);
  m_musicVolumeController.SetVolume(GetFractionalVolume(value));
}
