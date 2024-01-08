#include "pch.h"
#include "game_volume_controller.h"
#include "sound_data.h"
#include "game_settings.h"

game_volume_controller::game_volume_controller()
{
  auto effectBuffers = std::array
  {
    sound_data::get(sound_data::shoot),
    sound_data::get(sound_data::thrust),
    sound_data::get(sound_data::target_activated),
    sound_data::get(sound_data::mine_exploded)
  };
  
  m_effectsVolumeController = volume_controller { effectBuffers };

  auto musicBuffers = std::array
  {
    sound_data::get(sound_data::menu_theme)
  };

  m_musicVolumeController = volume_controller { musicBuffers };
}

auto game_volume_controller::minVolume() -> int
{
  return 0;
}

auto game_volume_controller::maxVolume() -> int
{
  return 10;
}

auto game_volume_controller::setEffectsVolume(int value) -> int
{
  value = std::max(value, minVolume());
  value = std::min(value, maxVolume());
  int volume = game_settings::setEffectsVolume(value);
  if( m_instance ) m_instance->SetEffectsVolume(volume);
  return volume;
}

auto game_volume_controller::setMusicVolume(int value) -> int
{
  value = std::max(value, minVolume());
  value = std::min(value, maxVolume());
  int volume = game_settings::setMusicVolume(value);
  if( m_instance ) m_instance->SetMusicVolume(volume);
  return volume;
}

auto game_volume_controller::effectsVolume() -> int
{
  return game_settings::effectsVolume();
}

auto game_volume_controller::musicVolume() -> int
{
  return game_settings::musicVolume();
}
