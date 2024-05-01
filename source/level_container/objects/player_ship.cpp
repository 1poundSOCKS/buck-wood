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

  if( m_destination )
  {
    bool atDestination = MoveTowards(800 * interval, *m_destination);
    m_destination = atDestination ? std::nullopt : m_destination;
  }
  else
  {
    // auto direction = DirectionalControlPressed();

    // auto cellId = cells.CellId(m_position);

    // auto newCellId = cellId;
    // auto& [column, row] = newCellId;

    // switch( direction )
    // {
    // case player_ship::control_direction_type::up:
    //   row -= cells.IsTypeOf({column, row - 1}, level_cell_collection::cell_type::floor) ? 1 : 0;
    //   break;
    // case player_ship::control_direction_type::down:
    //   row += cells.IsTypeOf({column, row + 1}, level_cell_collection::cell_type::floor) ? 1 : 0;
    //   break;
    // case player_ship::control_direction_type::left:
    //   column -= cells.IsTypeOf({column - 1, row}, level_cell_collection::cell_type::floor) ? 1 : 0;
    //   break;
    // case player_ship::control_direction_type::right:
    //   column += cells.IsTypeOf({column + 1, row}, level_cell_collection::cell_type::floor) ? 1 : 0;
    //   break;
    // }

    // if( newCellId != cellId )
    // {
    //   auto newCellIterator = cells.Get().find(newCellId);
      
    //   if( newCellIterator != std::end(cells.Get()) )
    //   {
    //     const auto& [key, value] = *newCellIterator;
    //     m_destination = value.Position();
    //   }
    // }    
      std::optional<D2D1_POINT_2F> leftThumbstickPosition = gamepad_reader::left_thumbstick();
      if( leftThumbstickPosition )
      {
        m_position = direct2d::CalculatePosition(m_position, {leftThumbstickPosition->x * 1000, leftThumbstickPosition->y * 1000}, interval);        
      }

  }

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  m_shootAngle = rightThumbstickPosition ? direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition) : m_shootAngle;
  m_triggerDown = rightThumbstickPosition ? true : false;
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
