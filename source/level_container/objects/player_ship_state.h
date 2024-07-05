#pragma once

#include "base_object.h"
#include "reload_counter.h"
#include "health_status.h"
#include "cell_size.h"
#include "fractional_counter.h"
#include "object_cell_position.h"

class player_ship_state : public base_object
{

public:

  player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept;

  auto Update(float interval, cell_size cellSize) -> void;

  auto MoveLeft() noexcept -> void;
  auto MoveRight() noexcept -> void;
  auto MoveUp() noexcept -> void;
  auto MoveDown() noexcept -> void;
  auto StayPut() noexcept -> void;

  auto SetCellId(cell_id cellId, cell_size cellSize) noexcept -> void;

  auto ApplyDamage(int value) -> void;
  auto ApplyFatalDamage() -> void;

  [[nodiscard]] auto ShieldStatus() const -> const health_status&;
  [[nodiscard]] auto CanShoot() -> bool;

private:

  enum class move_direction { none, up, down, left, right };

private:

  auto UpdateWhenActive(float interval, cell_size cellSize) -> void;
  auto UpdateWhenCelebrating(float interval) -> void;

private:

  health_status m_shieldStatus { 10 };
  reload_counter m_playerReloadCounter { 1.0f / 3.0f, 1 };
  move_direction m_moveDirection;
  object_cell_position m_cellPosition;

};

inline player_ship_state::player_ship_state(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept :
  base_object { position, scale, angle }, m_cellPosition { cell_id {0,0}, 0.5f }, m_moveDirection { move_direction::none }
{
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

inline auto player_ship_state::SetCellId(cell_id cellId, cell_size cellSize) noexcept -> void
{
  m_cellPosition.Set(cellId);
  m_position = m_cellPosition(0, object_cell_position::move_direction::none, cellSize);
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
