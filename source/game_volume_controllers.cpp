#include "pch.h"
#include "game_volume_controllers.h"
#include "sound_data.h"
#include "game_settings.h"

game_volume_controllers* game_volume_controllers::m_instance = nullptr;

game_volume_controllers::game_volume_controllers()
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

  SetEffectsVolume(game_settings::effectsVolume());
  SetMusicVolume(game_settings::musicVolume());
}

auto game_volume_controllers::setEffectsVolume(int value) -> void
{
  if( m_instance )
  {
    m_instance->SetEffectsVolume(value);
    game_settings::setEffectsVolume(value);
  }
}

auto game_volume_controllers::setMusicVolume(int value) -> void
{
  if( m_instance )
  {
    m_instance->SetMusicVolume(value);
    game_settings::setMusicVolume(value);
  }
}
