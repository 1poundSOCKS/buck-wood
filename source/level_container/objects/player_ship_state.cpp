#include "pch.h"
#include "player_ship_state.h"
#include "player_state.h"

auto player_ship_state::Update(float interval, cell_size cellSize) -> void
{
  switch(  player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(interval, cellSize);
      break;
    case  player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship_state::UpdateWhenActive(float interval, cell_size cellSize) -> void
{
  base_object::Update(interval);
  
  m_playerReloadCounter.Update(interval);

  switch( m_state )
  {
    case state::moving:
      UpdateWhenMoving(interval, cellSize);
      break;
    case state::waiting:
      UpdateWhenWaiting(interval);
      break;
  }
}

auto player_ship_state::UpdateWhenMoving(float interval, cell_size cellSize) -> void
{
  auto moveDirection = object_cell_position::move_direction::none;

  switch( m_moveDirection )
  {
    case move_direction::up:
      moveDirection = object_cell_position::move_direction::up;
      break;
    case move_direction::down:
      moveDirection = object_cell_position::move_direction::down;
      break;
    case move_direction::left:
      moveDirection = object_cell_position::move_direction::left;
      break;
    case move_direction::right:
      moveDirection = object_cell_position::move_direction::right;
      break;
  }

  m_position = m_cellPosition(interval, moveDirection, cellSize);
}

auto player_ship_state::UpdateWhenWaiting(float interval) -> void
{
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
