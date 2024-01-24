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

  static auto TargetActivated() -> void
  {
    m_instance->m_targetActivated.Start();
  }

  static auto Explosion() -> void
  {
    m_instance->m_explosion.Start();
  }

private:

  inline static audio_events* m_instance { nullptr };
  audio_player m_menuTheme { audio_data_item::menu_theme, audio_player::type::music };
  audio_player m_mainTheme { audio_data_item::main_theme, audio_player::type::music };
  audio_player m_playerThruster { audio_data_item::thruster, audio_player::type::music };
  audio_player m_playerShot { audio_data_item::shoot, audio_player::type::effect };
  audio_player m_targetActivated { audio_data_item::target_activated, audio_player::type::effect };
  audio_player m_explosion { audio_data_item::explosion, audio_player::type::effect };

};
