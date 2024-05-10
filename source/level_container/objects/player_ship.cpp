#include "pch.h"
#include "player_ship.h"
#include "player_state.h"

player_ship::player_ship(POINT_2F position) : 
  base_object { position, { 1.5f, 1.5f }, 0 }, m_shootAngle { 0 }
{
}

auto player_ship::Update(float interval, const level_cell_collection& cells) -> void
{
  switch( player_state::get_status() )
  {
    case player_state::status::active:
      UpdateWhenActive(interval, cells);
      break;
    case player_state::status::celebrating:
      UpdateWhenCelebrating(interval);
      break;
  }
}

auto player_ship::UpdateWhenActive(float interval, const level_cell_collection& cells) -> void
{
  m_playerReloadCounter.Update(interval);

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();

  std::optional<POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
  if( leftThumbstickPosition )
  {
    UpdatePosition(interval, cells, *leftThumbstickPosition);
  }

  m_triggerDown = rightThumbstickPosition != std::nullopt;

  if( m_triggerDown )
  {
    auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
    m_shootAngle = static_cast<float>(shootAngle);
  }

  // auto currentCellId = cells.CellId(m_position);
  // auto [currentColumn, currentRow] = currentCellId;

  // auto width = 90.0f;
  // auto height = 90.0f;

  // auto aboveCellId = level_cell_collection::cell_id { currentColumn, currentRow - 1 };
  // auto belowCellId = level_cell_collection::cell_id { currentColumn, currentRow + 1 };
  // auto leftCellId = level_cell_collection::cell_id { currentColumn - 1, currentRow };
  // auto rightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow };
  // auto aboveLeftCellId = level_cell_collection::cell_id { currentColumn - 1, currentRow - 1};
  // auto belowLeftCellId = level_cell_collection::cell_id { currentColumn - 1, currentRow + 1};
  // auto aboveRightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow - 1};
  // auto belowRightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow + 1};

  // auto aboveCellRect = cells.CellRect(aboveCellId);
  // auto belowCellRect = cells.CellRect(belowCellId);
  // auto leftCellRect = cells.CellRect(leftCellId);
  // auto rightCellRect = cells.CellRect(rightCellId);
  // auto aboveLeftCellRect = cells.CellRect(aboveLeftCellId);
  // auto belowLeftCellRect = cells.CellRect(belowLeftCellId);
  // auto aboveRightCellRect = cells.CellRect(aboveRightCellId);
  // auto belowRightCellRect = cells.CellRect(belowRightCellId);

  // auto leftCellOverlap = leftCellRect.right - ( m_position.x - width );
  // auto rightCellOverlap = m_position.x + width - rightCellRect.left;

  // auto aboveCellOverlap = aboveCellRect.bottom - ( m_position.y - height );
  // auto belowCellOverlap = m_position.y + height - belowCellRect.top;

  // if( cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = std::max(m_position.x - width, leftCellRect.right) + width;
  // }

  // if( cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::max(m_position.y - height, aboveCellRect.bottom) + height;
  // }

  // if( cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = std::min(m_position.x + width, rightCellRect.left) - width;
  // }

  // if( cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::min(m_position.y + height, belowCellRect.top) - height;
  // }

  // if( !cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) &&
  //     !cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) && 
  //     cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) )
  // {
  //   if( leftCellOverlap > aboveCellOverlap )
  //   {
  //     m_position.y = std::max(m_position.y - height, aboveLeftCellRect.bottom) + height;
  //   }
  //   else
  //   {
  //     m_position.x = std::max(m_position.x - width, aboveLeftCellRect.right) + width;
  //   }
  // }

  // if( cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall) )
  // {
  //   if( rightCellOverlap > belowCellOverlap )
  //   {
  //     m_position.y = std::min(m_position.y + height, belowRightCellRect.top) - height;
  //   }
  //   else
  //   {
  //     m_position.x = std::min(m_position.x + width, belowRightCellRect.left) - width;
  //   }
  // }

  // if( cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) )
  // {
  //   if( leftCellOverlap > belowCellOverlap )
  //   {
  //       m_position.y = std::min(m_position.y + height, belowCellRect.top) - height;
  //   }
  //   else
  //   {
  //     m_position.x = std::max(m_position.x - width, leftCellRect.right + 1) + width;
  //   }
  // }

  // bool wallBelow = cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) ||
  //   leftCellOverlap && cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) ||
  //   rightCellOverlap && cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall);

  // if( wallBelow )
  // {
  //   m_position.y = std::min(m_position.y + height, belowCellRect.top - 1) - height;
  // }

  // bool aboveCellOverlap = m_position.y - height < aboveCellRect.bottom;
  // bool belowCellOverlap = m_position.y + height > belowCellRect.top;

  // bool wallLeft = cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) ||
  //   aboveCellOverlap && cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) ||
  //   belowCellOverlap && cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall);

  // if( wallLeft )
  // {
  //   m_position.x = std::max(m_position.x - width, leftCellRect.right + 1) + width;
  // }

  // if( cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::max(m_position.y - height, aboveCellRect.bottom) + height;
  // }
  // else
  // {
  //   bool leftCellOverlap = m_position.x - width < leftCellRect.right;
  //   if( leftCellOverlap && cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) )
  //   {
  //     m_position.y = std::max(m_position.y - height, aboveLeftCellRect.bottom) + height;
  //   }
    
  //   bool rightCellOverlap = m_position.x + width > rightCellRect.left;
  //   if( rightCellOverlap && cells.IsTypeOf(aboveRightCellId, level_cell_collection::cell_type::wall) )
  //   {
  //     m_position.y = std::max(m_position.y - height, aboveRightCellRect.bottom) + height;
  //   }
  // }

  // if( cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::min(m_position.y + height, belowCellRect.top) - height;
  // }

  // if( cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = std::max(m_position.x - width, leftCellRect.right) + width;
  // }
  // else
  // {
  //   bool aboveCellOverlap = m_position.y - height < aboveCellRect.bottom;
  //   if( aboveCellOverlap && cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) )
  //   {
  //     m_position.x = std::max(m_position.x - width, aboveLeftCellRect.right) + width;
  //   }

  //   bool belowCellOverlap = m_position.y + height > belowCellRect.top;
  //   if( belowCellOverlap && cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall))
  //   {
  //     m_position.x = std::max(m_position.x - width, belowLeftCellRect.right) + width;
  //   }
  // }

  // if( cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = std::min(m_position.x + width, rightCellRect.left) - width;
  // }
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}

auto player_ship::UpdatePosition(float interval, const level_cell_collection& cells, POINT_2F movementControl) -> void
{
  auto moveDistance = POINT_2F { movementControl.x * 1000 * interval, movementControl.y * 1000 * interval };
  // auto updatedPosition = direct2d::CalculatePosition(m_position, {movementControl.x * 1000, movementControl.y * 1000}, interval);

  // auto updatedCellId = cells.CellId(updatedPosition);
  auto [updatedColumn, updatedRow] = cells.CellId(m_position);

  // auto width = 90.0f;
  // auto height = 90.0f;
  auto playerSize = SIZE_F { 90, 90 };

  auto aboveCellId = level_cell_collection::cell_id { updatedColumn, updatedRow - 1 };
  auto belowCellId = level_cell_collection::cell_id { updatedColumn, updatedRow + 1 };
  auto leftCellId = level_cell_collection::cell_id { updatedColumn - 1, updatedRow };
  auto rightCellId = level_cell_collection::cell_id { updatedColumn + 1, updatedRow };
  auto aboveLeftCellId = level_cell_collection::cell_id { updatedColumn - 1, updatedRow - 1};
  auto belowLeftCellId = level_cell_collection::cell_id { updatedColumn - 1, updatedRow + 1};
  // auto aboveRightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow - 1};
  // auto belowRightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow + 1};

  auto aboveCellRect = ExpandRect(cells.CellRect(aboveCellId), playerSize);
  auto belowCellRect = ExpandRect(cells.CellRect(belowCellId), playerSize);
  auto leftCellRect = ExpandRect(cells.CellRect(leftCellId), playerSize);
  auto rightCellRect = ExpandRect(cells.CellRect(rightCellId), playerSize);
  auto aboveLeftCellRect = ExpandRect(cells.CellRect(aboveLeftCellId), playerSize);
  auto belowLeftCellRect = ExpandRect(cells.CellRect(belowLeftCellId), playerSize);
  // auto aboveRightCellRect = cells.CellRect(aboveRightCellId);
  // auto belowRightCellRect = cells.CellRect(belowRightCellId);

  // auto leftCellOverlap = std::max(0.0f, leftCellRect.right - ( updatedPosition.x - width ));
  // auto rightCellOverlap = m_position.x + width - rightCellRect.left;

  // auto aboveCellOverlap = aboveCellRect.bottom - ( m_position.y - height );
  // auto belowCellOverlap = m_position.y + height - belowCellRect.top;

  auto wallLeft = cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) ? leftCellRect.right : leftCellRect.left;
  auto wallLeftAbove = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) && m_position.y < aboveLeftCellRect.bottom ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallLeftBelow = cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) && m_position.y > belowLeftCellRect.top ? belowLeftCellRect.right : belowLeftCellRect.left;
  // auto wallLeftAbove = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallRight = cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) ? rightCellRect.left : rightCellRect.right;

  wallLeft = std::max({wallLeft, wallLeftAbove, wallLeftBelow});

  auto minXShift = wallLeft  - m_position.x;
  auto maxXShift = wallRight - m_position.x;

  auto minYShift = cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) ? aboveCellRect.bottom - m_position.y : aboveCellRect.top - m_position.y;
  auto maxYShift = cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) ? belowCellRect.top - m_position.y : belowCellRect.bottom - m_position.y;

  moveDistance.x = std::max(moveDistance.x, minXShift);
  moveDistance.x = std::min(moveDistance.x, maxXShift);

  moveDistance.y = std::max(moveDistance.y, minYShift);
  moveDistance.y = std::min(moveDistance.y, maxYShift);

  m_position.x += moveDistance.x;
  m_position.y += moveDistance.y;

  // if( updatedPosition.x < m_position.x )
  // {
  //   if( cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) )
  //   {
  //     m_position.x = updatedPosition.x + leftCellOverlap;
  //   }
  //   else if( aboveCellOverlap > 0 && cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) )
  //   {
  //     m_position.x = updatedPosition.x + leftCellOverlap;
  //   }
  //   else
  //   {
  //     m_position.x = updatedPosition.x;
  //   }
  // }
  // else
  // {
  //   m_position.x = updatedPosition.x;
  // }

  // if( leftCellOverlap > 0 && cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = position.x;
  // }

  // if( cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::max(m_position.y - height, aboveCellRect.bottom) + height;
  // }

  // if( cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.x = std::min(m_position.x + width, rightCellRect.left) - width;
  // }

  // if( cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) )
  // {
  //   m_position.y = std::min(m_position.y + height, belowCellRect.top) - height;
  // }
}

[[nodiscard]] auto player_ship::GetUpdatedAngle(D2D1_POINT_2F position, float direction, D2D1_POINT_2F destination, float interval) -> float
{
  constexpr float rotationSpeed = 360.0f;
  auto rotationAmount = rotationSpeed * interval;

  auto angleToDestination = direct2d::GetAngleBetweenPoints(position, destination);
  auto angleDifference = direct2d::GetAngleDifference(direction, angleToDestination);

  auto rotationAngle = angleDifference < 0 ? -rotationAmount : rotationAmount;
  auto updatedAngle = direction + rotationAngle;
  updatedAngle = updatedAngle < 0 ? updatedAngle + 360 : updatedAngle;
  return updatedAngle < 360 ? updatedAngle : updatedAngle - 360;
}

[[nodiscard]] auto player_ship::GetUpdatedPosition(D2D1_POINT_2F position, VELOCITY_2F velocity, float interval) -> D2D1_POINT_2F
{
  return { position.x + velocity.x * interval, position.y + velocity.y * interval };
}

[[nodiscard]] auto player_ship::DirectionalControlPressed() const -> control_direction_type
{
  if( gamepad_reader::button_pressed(XINPUT_GAMEPAD_DPAD_UP) )
  {
    return control_direction_type::up;
  }

  if( gamepad_reader::button_pressed(XINPUT_GAMEPAD_DPAD_DOWN) )
  {
    return control_direction_type::down;
  }

  if( gamepad_reader::button_pressed(XINPUT_GAMEPAD_DPAD_LEFT) )
  {
    return control_direction_type::left;
  }

  if( gamepad_reader::button_pressed(XINPUT_GAMEPAD_DPAD_RIGHT) )
  {
    return control_direction_type::right;
  }

  return control_direction_type::none;
}

auto player_ship::Visit(adjacent_cell_visitor& visitor, const level_cell_collection& cellCollection) -> void
{
  visitor(cellCollection, [](const valid_cell& cell)
  {
  });
}

[[nodiscard]] auto player_ship::ExpandRect(RECT_F rect, SIZE_F size) -> RECT_F
{
  return { rect.left - size.width, rect.top - size.height, rect.right + size.width, rect.bottom + size.height };
}
