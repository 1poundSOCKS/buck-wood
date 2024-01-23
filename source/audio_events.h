#pragma once

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

  static auto StartMainMenuTheme() -> void {}
  static auto StopMainMenuTheme() -> void {}

  static auto StartGameplayTheme() -> void {}
  static auto StopGameplayTheme() -> void {}

  static auto StartPlayerThruster() -> void {}
  static auto StopPlayerThruster() -> void {}
  static auto PlayerShot() -> void {}
  static auto TargetActivated() -> void {}
  static auto Explosion() -> void {}

private:

  inline static audio_events* m_instance { nullptr };

};
