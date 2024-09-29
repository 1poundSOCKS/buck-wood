#include "pch.h"
#include "player_controls.h"

player_controls::player_controls(std::shared_ptr<player_ship_state> state, std::shared_ptr<level_cell_collection> cells) noexcept : m_state { state }, m_cells { cells }
{
}

auto player_controls::SetState(std::shared_ptr<player_ship_state> state) noexcept -> void
{
  m_state = state;
}

auto player_controls::Update(float interval) const noexcept -> void
{
  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( leftThumbstickPosition )
  {
    auto position = m_state->Position();
    auto positionOffset = POINT_2F { leftThumbstickPosition->x * 500.0f * interval, leftThumbstickPosition->y * 500.0f * interval };
    auto newPosition = POINT_2F { position.x + positionOffset.x, position.y + positionOffset.y };
    m_state->SetPosition(newPosition);
  }

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  rightThumbstickPosition ? m_state->SetShootAngle(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition)) : m_state->ResetShootAngle();
}
