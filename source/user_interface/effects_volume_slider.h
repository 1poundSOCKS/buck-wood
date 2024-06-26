#pragma once

#include "setting_slider.h"
#include "game_volume_controller.h"
#include "audio_events.h"

class effects_volume_slider : public setting_slider
{

public:

  effects_volume_slider();

  [[nodiscard]] virtual auto Min() const -> int;
  [[nodiscard]] virtual auto Max() const -> int;
  [[nodiscard]] virtual auto Value() const -> int;

  virtual auto Increment() -> int;
  virtual auto Decrement() -> int;

};

inline effects_volume_slider::effects_volume_slider()
{
  m_name = L"Effects volume";
}

[[nodiscard]] inline auto effects_volume_slider::Min() const -> int
{
  return game_volume_controller::minVolume();
}

[[nodiscard]] inline auto effects_volume_slider::Max() const -> int
{
  return game_volume_controller::maxVolume();
}

[[nodiscard]] inline auto effects_volume_slider::Value() const -> int
{
  return game_volume_controller::effectsVolume();
}

inline auto effects_volume_slider::Increment() -> int
{
  auto volume = game_volume_controller::effectsVolume();
  volume = game_volume_controller::setEffectsVolume(volume + 1);
  audio_events::PowerUpCollected();
  return volume;
}

inline auto effects_volume_slider::Decrement() -> int
{
  auto volume = game_volume_controller::effectsVolume();
  volume = game_volume_controller::setEffectsVolume(volume - 1);
  audio_events::PowerUpCollected();
  return volume;
}
