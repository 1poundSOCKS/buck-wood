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

  m_movementState = gamepad_reader::right_trigger() > 0 ? movement_state::dash : movement_state::normal;

  switch( m_movementState )
  {
    case movement_state::normal:
      {
        std::optional<POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
        if( leftThumbstickPosition )
        {
          auto moveDistance = POINT_2F { leftThumbstickPosition->x * 500 * interval, leftThumbstickPosition->y * 500 * interval };
          UpdatePosition(interval, cells, moveDistance);
        }

        m_triggerDown = rightThumbstickPosition != std::nullopt;

        if( m_triggerDown )
        {
          auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
          m_shootAngle = static_cast<float>(shootAngle);
        }

        // if( leftThumbstickPosition && gamepad_reader::button_pressed(XINPUT_GAMEPAD_A) )
        // if( leftThumbstickPosition )
        // {
        //   m_movementState = movement_state::dash;
        //   auto leftThumbstickDirection = direct2d::CalculateDirection({leftThumbstickPosition->x, leftThumbstickPosition->y});
        //   m_dashVelocity = direct2d::CalculateVelocity(2000, leftThumbstickDirection);
        // }

      }

      break;

    case movement_state::dash:

      // m_dashTimer.Update(interval);

      // if( m_dashTimer.Get(1, true) > 0 )
      // {
      //   m_movementState = movement_state::normal;
      // }
      // else
      // {
      //   auto moveDistance = direct2d::CalculatePosition({0,0}, m_dashVelocity, interval);
      //   UpdatePosition(interval, cells, moveDistance);
      // }
      {
        std::optional<POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();

        if( leftThumbstickPosition )
        {
          auto moveDistance = POINT_2F { leftThumbstickPosition->x * 1500 * interval, leftThumbstickPosition->y * 1500 * interval };
          UpdatePosition(interval, cells, moveDistance);
        }
      }

      break;

  }
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
}

auto player_ship::UpdatePosition(float interval, const level_cell_collection& cells, POINT_2F moveDistance) -> void
{
  // auto moveDistance = POINT_2F { movementControl.x * 500 * interval, movementControl.y * 500 * interval };

  auto [column, row] = cells.CellId(m_position);

  auto playerSize = SIZE_F { 90, 90 };

  auto aboveCellId = level_cell_collection::cell_id { column, row - 1 };
  auto belowCellId = level_cell_collection::cell_id { column, row + 1 };
  auto leftCellId = level_cell_collection::cell_id { column - 1, row };
  auto rightCellId = level_cell_collection::cell_id { column + 1, row };
  auto aboveLeftCellId = level_cell_collection::cell_id { column - 1, row - 1};
  auto belowLeftCellId = level_cell_collection::cell_id { column - 1, row + 1};
  auto aboveRightCellId = level_cell_collection::cell_id { column + 1, row - 1};
  auto belowRightCellId = level_cell_collection::cell_id { column + 1, row + 1};

  auto aboveCellRect = ExpandRect(cells.CellRect(aboveCellId), playerSize);
  auto belowCellRect = ExpandRect(cells.CellRect(belowCellId), playerSize);
  auto leftCellRect = ExpandRect(cells.CellRect(leftCellId), playerSize);
  auto rightCellRect = ExpandRect(cells.CellRect(rightCellId), playerSize);
  auto aboveLeftCellRect = ExpandRect(cells.CellRect(aboveLeftCellId), playerSize);
  auto belowLeftCellRect = ExpandRect(cells.CellRect(belowLeftCellId), playerSize);
  auto aboveRightCellRect = ExpandRect(cells.CellRect(aboveRightCellId), playerSize);
  auto belowRightCellRect = ExpandRect(cells.CellRect(belowRightCellId), playerSize);

  auto wallLeft = cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) ? leftCellRect.right : leftCellRect.left;
  auto wallLeftAbove = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) && m_position.y < aboveLeftCellRect.bottom ? aboveLeftCellRect.right : aboveLeftCellRect.left;
  auto wallLeftBelow = cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) && m_position.y > belowLeftCellRect.top ? belowLeftCellRect.right : belowLeftCellRect.left;

  auto wallAbove = cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) ? aboveCellRect.bottom : aboveCellRect.top;
  auto wallAboveLeft = cells.IsTypeOf(aboveLeftCellId, level_cell_collection::cell_type::wall) && m_position.x < aboveLeftCellRect.right ? aboveLeftCellRect.bottom : aboveLeftCellRect.top;
  auto wallAboveRight = cells.IsTypeOf(aboveRightCellId, level_cell_collection::cell_type::wall) && m_position.x > aboveRightCellRect.left ? aboveRightCellRect.bottom : aboveRightCellRect.top;

  auto wallRight = cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) ? rightCellRect.left : rightCellRect.right;
  auto wallRightAbove = cells.IsTypeOf(aboveRightCellId, level_cell_collection::cell_type::wall) && m_position.y < aboveRightCellRect.bottom ? aboveRightCellRect.left : aboveRightCellRect.right;
  auto wallRightBelow = cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall) && m_position.y > belowRightCellRect.top ? belowRightCellRect.left : belowRightCellRect.right;

  auto wallBelow = cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) ? belowCellRect.top : belowCellRect.bottom;
  auto wallBelowLeft = cells.IsTypeOf(belowLeftCellId, level_cell_collection::cell_type::wall) && m_position.x < belowLeftCellRect.right ? belowLeftCellRect.top : belowLeftCellRect.bottom;
  auto wallBelowRight = cells.IsTypeOf(belowRightCellId, level_cell_collection::cell_type::wall) && m_position.x > belowRightCellRect.left ? belowRightCellRect.top : belowRightCellRect.bottom;

  wallLeft = std::max({wallLeft, wallLeftAbove, wallLeftBelow});
  wallAbove = std::max({wallAbove, wallAboveLeft, wallAboveRight});
  wallRight = std::min({wallRight, wallRightAbove, wallRightBelow});
  wallBelow = std::min({wallBelow, wallBelowLeft, wallBelowRight});

  auto minXShift = wallLeft  - m_position.x;
  auto maxXShift = wallRight - m_position.x;

  auto minYShift = wallAbove - m_position.y;
  auto maxYShift = wallBelow - m_position.y;

  moveDistance.x = std::max(moveDistance.x, minXShift);
  moveDistance.x = std::min(moveDistance.x, maxXShift);

  moveDistance.y = std::max(moveDistance.y, minYShift);
  moveDistance.y = std::min(moveDistance.y, maxYShift);

  m_position.x += moveDistance.x;
  m_position.y += moveDistance.y;
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
