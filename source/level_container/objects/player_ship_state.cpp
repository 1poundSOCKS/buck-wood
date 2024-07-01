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
  Update();
  m_playerReloadCounter.Update(interval);

  if( m_currentCellId == m_nextCellId )
  {
    switch( m_moveDirection )
    {
      case move_direction::up:
        m_nextCellId = m_currentCellId.Get(cell_id::relative_position::above);
        break;
      case move_direction::down:
        m_nextCellId = m_currentCellId.Get(cell_id::relative_position::below);
        break;
      case move_direction::left:
        m_nextCellId = m_currentCellId.Get(cell_id::relative_position::left);
        break;
      case move_direction::right:
        m_nextCellId = m_currentCellId.Get(cell_id::relative_position::right);
        break;
    }

    m_moveTimer.Reset();
  }
  else
  {
    auto moveTimer = m_moveTimer.Update(interval);

    if( moveTimer < 1.0f )
    {
      auto startPosition = ToFloat(cellSize.CellPosition(m_currentCellId));
      auto endPosition = ToFloat(cellSize.CellPosition(m_nextCellId));

      auto distanceToTravel = POINT_2F { endPosition.x - startPosition.x, endPosition.y - startPosition.y };
      auto distanceTravelled = POINT_2F { distanceToTravel.x * moveTimer, distanceToTravel.y * moveTimer };
      m_position = { startPosition.x + distanceTravelled.x, startPosition.y + distanceTravelled.y };
    }
    else
    {
      m_currentCellId = m_nextCellId;
      m_position = ToFloat(cellSize.CellPosition(m_currentCellId));
    }
  }
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}

auto player_ship_state::UpdateVelocity(VELOCITY_2F changeInVelocity, float interval) -> void
{
  auto currentVelocity = m_velocity;
  m_velocity = VELOCITY_2F { currentVelocity.x + changeInVelocity.x * interval, currentVelocity.y + changeInVelocity.y * interval };
}

auto player_ship_state::UpdateAngle() -> void
{
  m_angle = direct2d::CalculateDirection(m_velocity);
}
