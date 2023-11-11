#pragma once

#include "setting_slider.h"

class music_volume_slider : public setting_slider
{

  [[nodiscard]] virtual auto Min() const -> int;
  [[nodiscard]] virtual auto Max() const -> int;
  [[nodiscard]] virtual auto Value() const -> int;

  virtual auto Increment() -> int;
  virtual auto Decrement() -> int;

};

[[nodiscard]] inline auto music_volume_slider::Min() const -> int
{
  return 0;
}

[[nodiscard]] inline auto music_volume_slider::Max() const -> int
{
  return 10;
}

[[nodiscard]] inline auto music_volume_slider::Value() const -> int
{
  return game_volume_controllers::musicVolume();
}

inline auto music_volume_slider::Increment() -> int
{
  auto volume = game_volume_controllers::musicVolume();
  return game_volume_controllers::setMusicVolume(volume + 1);
}

inline auto music_volume_slider::Decrement() -> int
{
  auto volume = game_volume_controllers::musicVolume();
  return game_volume_controllers::setMusicVolume(volume - 1);
}
