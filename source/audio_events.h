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

  static auto StartGameplayTheme() -> void {}
  static auto StopGameplayTheme() -> void {}

  static auto StartPlayerThruster() -> void {}
  static auto StopPlayerThruster() -> void {}
  static auto PlayerShot() -> void {}
  static auto TargetActivated() -> void {}
  static auto Explosion() -> void {}

private:

  inline static audio_events* m_instance { nullptr };
  audio_player m_menuTheme { audio_data_item::menu_theme };

};
