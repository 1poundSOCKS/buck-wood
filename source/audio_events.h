#pragma once

class audio_events
{

public:

  static auto StartMainMenuTheme() -> void {}
  static auto StopMainMenuTheme() -> void {}

  static auto StartGameplayTheme() -> void {}
  static auto StopGameplayTheme() -> void {}

  static auto StartPlayerThruster() -> void {}
  static auto StopPlayerThruster() -> void {}
  static auto PlayerShot() -> void {}
  static auto TargetActivated() -> void {}
  static auto Explosion() -> void {}

};
