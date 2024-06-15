#pragma once

#include "base_object.h"

class player_ship_state : public base_object
{

public:

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle) noexcept;

  auto Update() noexcept -> void;

  [[nodiscard]] auto ThrusterOn() const noexcept -> bool;

private:

  bool m_thrustControlActivated { false };

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle) noexcept :
  base_object { position, scale, angle }
{
}

inline auto player_ship_state::ThrusterOn() const noexcept -> bool
{
  return !m_destroyed && m_thrustControlActivated;
}

inline auto player_ship_state::Update() noexcept -> void
{
  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  m_thrustControlActivated = rightThumbstickPosition != std::nullopt;
}
