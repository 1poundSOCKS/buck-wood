#pragma once

#include "base_object.h"
#include "reload_counter.h"
#include "health_status.h"
#include "cell_size.h"
#include "fractional_counter.h"

class player_ship_state : public base_object
{

public:

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept;

  auto Update(float interval, cell_size cellSize) -> void;
  auto UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void;
  auto UpdateAngle() -> void;
  auto UpdatePosition(float interval) -> POINT_2F;

  auto Update() noexcept -> void;
  auto SetVelocity(VELOCITY_2F value) -> void;

  auto MoveLeft() noexcept -> void;
  auto MoveRight() noexcept -> void;
  auto MoveUp() noexcept -> void;
  auto MoveDown() noexcept -> void;
  auto StayPut() noexcept -> void;

  auto SetCellId(cell_id cellId) noexcept -> void;

  [[nodiscard]] auto Velocity() const noexcept -> VELOCITY_2F;
  [[nodiscard]] auto ThrusterOn() const noexcept -> bool;

  auto Rotate(float angle) -> void;
  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto CanShoot() -> bool;

  static [[nodiscard]] auto CalculateVelocity(float controlX, float controlY) -> VELOCITY_2F;

private:

  enum class move_direction { none, up, down, left, right };

private:

  auto UpdateWhenActive(float interval, cell_size cellSize) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  static constexpr float m_thrustPower { 3000.0f };
  static constexpr float m_friction { 0.05f };
  static constexpr SIZE_F m_objectSize { 60, 60 };

private:

  health_status m_shieldStatus { 10 };
  reload_counter m_moveCounter { 1.0f / 3.0f, 1 };
  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  fractional_counter m_moveTimer;

private:

  VELOCITY_2F m_velocity;
  bool m_thrustControlActivated { false };
  move_direction m_moveDirection;

  cell_id m_currentCellId;
  cell_id m_nextCellId;

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept :
  base_object { position, scale, angle }, m_velocity { velocity }, m_moveTimer { 0.5f }, m_moveDirection { move_direction::none }
{
}

inline auto player_ship_state::Update() noexcept -> void
{
  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  m_thrustControlActivated = rightThumbstickPosition != std::nullopt;
}

inline auto player_ship_state::SetVelocity(VELOCITY_2F value) -> void
{
  m_velocity = value;
}

inline auto player_ship_state::MoveLeft() noexcept -> void
{
  m_moveDirection = move_direction::left;
}

inline auto player_ship_state::MoveRight() noexcept -> void
{
  m_moveDirection = move_direction::right;
}

inline auto player_ship_state::MoveUp() noexcept -> void
{
  m_moveDirection = move_direction::up;
}

inline auto player_ship_state::MoveDown() noexcept -> void
{
  m_moveDirection = move_direction::down;
}

inline auto player_ship_state::StayPut() noexcept -> void
{
  m_moveDirection = move_direction::none;
}

inline auto player_ship_state::SetCellId(cell_id cellId) noexcept -> void
{
  m_currentCellId = m_nextCellId = cellId;
}

inline auto player_ship_state::UpdatePosition(float interval) -> POINT_2F
{
  m_position.x += m_velocity.x * interval;
  m_position.y += m_velocity.y * interval;
  return m_position;
}

inline [[nodiscard]] auto player_ship_state::Velocity() const noexcept -> VELOCITY_2F
{
  return m_velocity;
}

inline auto player_ship_state::ThrusterOn() const noexcept -> bool
{
  return !m_destroyed && m_thrustControlActivated;
}

inline auto player_ship_state::Rotate(float angle) -> void
{
  m_angle = direct2d::RotateAngle(m_angle, angle);
}

inline auto player_ship_state::ApplyDamage(int value) -> void
{
  if( m_shieldStatus.ApplyDamage(value) == 0 )
  {
    Destroy();
  }
}

inline auto player_ship_state::ApplyFatalDamage() -> void
{
  m_shieldStatus.ApplyFatalDamage();
  Destroy();
}

inline [[nodiscard]] auto player_ship_state::ShieldStatus() const -> const health_status&
{
  return m_shieldStatus;
}

inline [[nodiscard]] auto player_ship_state::CanShoot() -> bool
{
  return !m_destroyed && m_playerReloadCounter.Get(1, true) == 1;
}

inline auto player_ship_state::CalculateVelocity(float controlX, float controlY) -> VELOCITY_2F
{
  constexpr float thrustPower { 3000.0f };
  return { controlX * thrustPower, controlY * thrustPower };
}
