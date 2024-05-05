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

  if( rightThumbstickPosition )
  {
    auto shootAngle = static_cast<int>(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition));
    m_shootAngle = static_cast<float>(shootAngle);
    m_triggerDown = true;
  }
  else
  {
    m_triggerDown = false;

    std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
    if( leftThumbstickPosition )
    {
      m_position = direct2d::CalculatePosition(m_position, {leftThumbstickPosition->x * 1000, leftThumbstickPosition->y * 1000}, interval);        
    }
  }

  auto currentCellId = cells.CellId(m_position);
  auto [currentColumn, currentRow] = currentCellId;

  auto aboveCellId = level_cell_collection::cell_id { currentColumn, currentRow - 1 };
  if( cells.IsTypeOf(aboveCellId, level_cell_collection::cell_type::wall) )
  {
    auto aboveCellRect = cells.CellRect(aboveCellId);
    auto height = 100.0f;
    m_position.y = std::max(m_position.y - height, aboveCellRect.bottom) + height;
  }

  auto belowCellId = level_cell_collection::cell_id { currentColumn, currentRow + 1 };
  if( cells.IsTypeOf(belowCellId, level_cell_collection::cell_type::wall) )
  {
    auto belowCellRect = cells.CellRect(belowCellId);
    auto height = 100.0f;
    m_position.y = std::min(m_position.y + height, belowCellRect.top) - height;
  }


  auto leftCellId = level_cell_collection::cell_id { currentColumn - 1, currentRow };
  if( cells.IsTypeOf(leftCellId, level_cell_collection::cell_type::wall) )
  {
    auto leftCellRect = cells.CellRect(leftCellId);
    auto width = 100.0f;
    m_position.x = std::max(m_position.x - width, leftCellRect.right) + width;
  }

  auto rightCellId = level_cell_collection::cell_id { currentColumn + 1, currentRow };
  if( cells.IsTypeOf(rightCellId, level_cell_collection::cell_type::wall) )
  {
    auto rightCellRect = cells.CellRect(rightCellId);
    auto width = 100.0f;
    m_position.x = std::min(m_position.x + width, rightCellRect.left) - width;
  }
}

auto player_ship::UpdateWhenCelebrating(float interval) -> void
{
  constexpr float rotationSpeed = 480.0f;
  auto rotationAmount = rotationSpeed * interval;
  m_angle = direct2d::RotateAngle(m_angle, rotationAmount);
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
