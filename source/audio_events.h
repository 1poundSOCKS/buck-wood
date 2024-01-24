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
    HRESULT hr = m_instance->m_menuTheme->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] StartMainMenuTheme");
  }

  static auto StopMainMenuTheme() -> void
  {
    HRESULT hr = m_instance->m_menuTheme->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopMainMenuTheme");
  }

  static auto StartGameplayTheme() -> void
  {
    HRESULT hr = m_instance->m_mainTheme->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] StartGameplayTheme");
  }

  static auto StopGameplayTheme() -> void
  {
    HRESULT hr = m_instance->m_mainTheme->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopGameplayTheme");
  }

  static auto StartPlayerThruster() -> void
  {
    HRESULT hr = m_instance->m_playerThruster->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] StartPlayerThruster");
  }

  static auto StopPlayerThruster() -> void
  {
    HRESULT hr = m_instance->m_playerThruster->Stop();
    com_logger::write(log::type::debug, hr, "[audio_events] StopPlayerThruster");
  }

  static auto PlayerShot() -> void
  {
    HRESULT hr = m_instance->m_playerShot->Stop();
    hr = m_instance->m_playerShot->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] PlayerShot");
  }

  static auto TargetActivated() -> void
  {
    HRESULT hr = m_instance->m_targetActivated->Stop();
    hr = m_instance->m_targetActivated->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] TargetActivated");
  }

  static auto Explosion() -> void
  {
    HRESULT hr = m_instance->m_explosion->Stop();
    hr = m_instance->m_explosion->Start(0, 0);
    com_logger::write(log::type::debug, hr, "[audio_events] Explosion");
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
