#include "pch.h"
#include "object_cell_position.h"

object_cell_position::object_cell_position(cell_id position, float moveInterval) : 
  m_currentPosition { position }, m_nextPosition { position }, m_moveInterval { moveInterval }
{
}

auto object_cell_position::Set(cell_id position) -> void
{
  m_currentPosition = m_nextPosition = position;
}

auto object_cell_position::operator()(float interval, move_direction moveDirection, cell_size cellSize) -> POINT_2F
{
  auto moveTimer = m_moveInterval.Update(interval);

  if( moveTimer >= 1.0f )
  {
    m_currentPosition = m_nextPosition;
    moveTimer = m_moveInterval.Normalize();
  }

  if( m_currentPosition == m_nextPosition )
  {
    switch( moveDirection )
    {
      case move_direction::up:
        m_nextPosition = m_currentPosition.Get(cell_id::relative_position::above);
        break;
      case move_direction::down:
        m_nextPosition = m_currentPosition.Get(cell_id::relative_position::below);
        break;
      case move_direction::left:
        m_nextPosition = m_currentPosition.Get(cell_id::relative_position::left);
        break;
      case move_direction::right:
        m_nextPosition = m_currentPosition.Get(cell_id::relative_position::right);
        break;
      case move_direction::none:
        m_moveInterval.Reset();
        break;
    }
  }

  auto startPosition = ToFloat(cellSize.CellPosition(m_currentPosition));
  auto endPosition = ToFloat(cellSize.CellPosition(m_nextPosition));

  auto distanceToTravel = POINT_2F { endPosition.x - startPosition.x, endPosition.y - startPosition.y };
  auto distanceTravelled = POINT_2F { distanceToTravel.x * moveTimer, distanceToTravel.y * moveTimer };
  return { startPosition.x + distanceTravelled.x, startPosition.y + distanceTravelled.y };
}

auto object_cell_position::Current() const noexcept -> cell_id
{
  return m_currentPosition;
}

auto object_cell_position::Next() const noexcept -> cell_id
{
  return m_nextPosition;
}

auto object_cell_position::MoveDirection(float direction) noexcept -> move_direction
{
  auto adjustedAngleToTarget = static_cast<int>(direction) + 45;
  auto normalizedAngleToTarget = adjustedAngleToTarget > 359 ? adjustedAngleToTarget - 360 : adjustedAngleToTarget;
  auto segmentedAngleToTarget = normalizedAngleToTarget / 90;

  switch( segmentedAngleToTarget )
  {
    case 0:
      return object_cell_position::move_direction::up;
    case 1:
      return object_cell_position::move_direction::right;
    case 2:
      return object_cell_position::move_direction::down;
    case 3:
      return object_cell_position::move_direction::left;
    default:
      return object_cell_position::move_direction::none;
  }
}

auto object_cell_position::MoveDirection(float x, float y) noexcept -> move_direction
{
  return MoveDirection(direct2d::GetAngle(POINT_2F { x, y }));
}
