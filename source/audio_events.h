#pragma once

#include "audio_data.h"
#include "audio_player.h"
#include "com_logger.h"

class audio_events
{

public:

  static auto create() -> void
  {
    destroy();
    m_instance = new audio_events();
  }

  static auto destroy() -> void
  {
    delete m_instance;
    m_instance = nullptr;
  }

  static auto SetMusicVolume(float value) -> void
  {
    HRESULT hr = m_instance->m_menuTheme->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_menuTheme->SetVolume");
    hr = m_instance->m_mainTheme->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_mainTheme->SetVolume");
  }

  static auto SetEffectsVolume(float value) -> void
  {
    HRESULT hr = m_instance->m_playerThruster->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_playerThruster->SetVolume");
    hr = m_instance->m_playerShot->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_playerShot->SetVolume");
    hr = m_instance->m_explosion->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_explosion->SetVolume");
    hr = m_instance->m_powerUpCollected->SetVolume(value);
    com_logger::write(log::type::debug, hr, "[audio_events::SetMusicVolume] m_powerUpCollected->SetVolume");
  }

  static auto StartMainMenuTheme() -> void
  {
    m_instance->m_menuTheme.Start();
  }

  static auto StopMainMenuTheme() -> void
  {
    HRESULT hr = m_instance->m_menuTheme->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopMainMenuTheme");
  }

  static auto StartGameplayTheme() -> void
  {
    m_instance->m_mainTheme.Start();
  }

  static auto StopGameplayTheme() -> void
  {
    HRESULT hr = m_instance->m_mainTheme->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopGameplayTheme");
  }

  static auto StartPlayerThruster() -> void
  {
    m_instance->m_playerThruster.Start();
  }

  static auto StopPlayerThruster() -> void
  {
    HRESULT hr = m_instance->m_playerThruster->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopPlayerThruster");
  }

  static auto PlayerShot() -> void
  {
    m_instance->m_playerShot.Start();
  }

  static auto Explosion() -> void
  {
    m_instance->m_explosion.Start();
  }

  static auto PowerUpCollected() -> void
  {
    m_instance->m_powerUpCollected.Start();
  }

private:

  inline static audio_events* m_instance { nullptr };
  audio_player m_menuTheme { audio_data_item::menu_theme, audio_player::type::music };
  audio_player m_mainTheme { audio_data_item::main_theme, audio_player::type::music };
  audio_player m_playerThruster { audio_data_item::thruster, audio_player::type::music };
  audio_player m_playerShot { audio_data_item::shoot, audio_player::type::effect };
  audio_player m_explosion { audio_data_item::explosion, audio_player::type::effect };
  audio_player m_powerUpCollected { audio_data_item::power_up, audio_player::type::effect };

};
