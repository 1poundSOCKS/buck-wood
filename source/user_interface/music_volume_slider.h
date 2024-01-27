#pragma once

#include "setting_slider.h"
#include "game_volume_controller.h"
#include "audio_events.h"

class music_volume_slider : public setting_slider
{

public:

  music_volume_slider();

private:

  [[nodiscard]] virtual auto Min() const -> int;
  [[nodiscard]] virtual auto Max() const -> int;
  [[nodiscard]] virtual auto Value() const -> int;

  virtual auto Increment() -> int;
  virtual auto Decrement() -> int;

  virtual auto Select() -> void;
  virtual auto Unselect() -> void;

};

inline music_volume_slider::music_volume_slider()
{
  m_name = L"Music volume";
}

[[nodiscard]] inline auto music_volume_slider::Min() const -> int
{
  return game_volume_controller::minVolume();
}

[[nodiscard]] inline auto music_volume_slider::Max() const -> int
{
  return game_volume_controller::maxVolume();
}

[[nodiscard]] inline auto music_volume_slider::Value() const -> int
{
  return game_volume_controller::musicVolume();
}

inline auto music_volume_slider::Increment() -> int
{
  auto volume = game_volume_controller::musicVolume();
  volume = game_volume_controller::setMusicVolume(volume + 1);
  return volume;
}

inline auto music_volume_slider::Decrement() -> int
{
  auto volume = game_volume_controller::musicVolume();
  volume = game_volume_controller::setMusicVolume(volume - 1);
  return volume;
}

inline auto music_volume_slider::Select() -> void
{
}

inline auto music_volume_slider::Unselect() -> void
{
}
