#pragma once

#include "base_object.h"
#include "object_velocity.h"

class player_ship_state : public base_object
{

public:

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle) noexcept;

  auto Update() noexcept -> void;
  auto SetVelocity(VELOCITY_2F value) -> void;

  auto UpdatePosition(float interval) -> POINT_2F;

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F;
  [[nodiscard]] auto ThrusterOn() const noexcept -> bool;

private:

  object_velocity m_velocity;
  bool m_thrustControlActivated { false };

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle) noexcept :
  base_object { position, scale, angle }
{
}

inline auto player_ship_state::Update() noexcept -> void
{
  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  m_thrustControlActivated = rightThumbstickPosition != std::nullopt;
}

inline auto player_ship_state::SetVelocity(VELOCITY_2F value) -> void
{
  m_velocity.Set(value.x, value.y);
}

inline auto player_ship_state::UpdatePosition(float interval) -> POINT_2F
{
  m_position.x += m_velocity.X() * interval;
  m_position.y += m_velocity.Y() * interval;
  return m_position;
}

inline [[nodiscard]] auto player_ship_state::Velocity() const noexcept -> VELOCITY_2F
{
  return m_velocity.Get();
}

inline auto player_ship_state::ThrusterOn() const noexcept -> bool
{
  return !m_destroyed && m_thrustControlActivated;
}
