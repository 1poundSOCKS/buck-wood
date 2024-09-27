#include "pch.h"
#include "player_controls.h"

player_controls::player_controls(std::shared_ptr<player_ship_state> state, std::shared_ptr<level_cell_collection> cells) noexcept : m_state { state }, m_cells { cells }
{
}

auto player_controls::SetState(std::shared_ptr<player_ship_state> state) noexcept -> void
{
  m_state = state;
}

auto player_controls::Update() const noexcept -> void
{
  m_state->StayPut();

  auto leftThumbstickPosition = gamepad_reader::left_thumbstick();

  if( leftThumbstickPosition )
  {
    enum class horizontal_move { none, left, right };
    enum class vertical_move { none, up, down };

    auto horizontalMove = horizontal_move::none;
    horizontalMove = leftThumbstickPosition->x > 0.3 ? horizontal_move::right : horizontalMove;
    horizontalMove = leftThumbstickPosition->x < -0.3 ? horizontal_move::left : horizontalMove;

    auto verticalMove = vertical_move::none;
    verticalMove = leftThumbstickPosition->y < -0.3 ? vertical_move::up : verticalMove;
    verticalMove = leftThumbstickPosition->y > 0.3 ? vertical_move::down : verticalMove;

    auto playerCellId = m_state->CellId();

    switch( horizontalMove )
    {
      case horizontal_move::left:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::left);
        m_cells->IsTypeOf(cellId, level_cell_type::floor) ? m_state->MoveLeft() : m_state->StayPut();
        break;
      }

      case horizontal_move::right:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::right);
        m_cells->IsTypeOf(cellId, level_cell_type::floor) ? m_state->MoveRight() : m_state->StayPut();
        break;
      }
    }

    switch( verticalMove )
    {
      case vertical_move::up:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::above);
        m_cells->IsTypeOf(cellId, level_cell_type::floor) ? m_state->MoveUp() : m_state->StayPut();
        break;
      }

      case vertical_move::down:
      {
        auto cellId = playerCellId.Get(cell_id::relative_position::below);
        m_cells->IsTypeOf(cellId, level_cell_type::floor) ? m_state->MoveDown() : m_state->StayPut();
        break;
      }
    }
  }

  std::optional<D2D1_POINT_2F> rightThumbstickPosition = gamepad_reader::right_thumbstick();
  rightThumbstickPosition ? m_state->SetShootAngle(direct2d::GetAngleBetweenPoints({0,0}, *rightThumbstickPosition)) : m_state->ResetShootAngle();
}
