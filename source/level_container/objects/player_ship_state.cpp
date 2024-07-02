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

  auto moveTimer = m_moveTimer.Update(interval);

  if( moveTimer >= 1.0f )
  {
    m_currentCellId = m_nextCellId;
    moveTimer = m_moveTimer.Normalize();
  }

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
      case move_direction::none:
        m_moveTimer.Reset();
        break;
    }
  }

  auto startPosition = ToFloat(cellSize.CellPosition(m_currentCellId));
  auto endPosition = ToFloat(cellSize.CellPosition(m_nextCellId));

  auto distanceToTravel = POINT_2F { endPosition.x - startPosition.x, endPosition.y - startPosition.y };
  auto distanceTravelled = POINT_2F { distanceToTravel.x * moveTimer, distanceToTravel.y * moveTimer };
  m_position = { startPosition.x + distanceTravelled.x, startPosition.y + distanceTravelled.y };
}

auto player_ship_state::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}
