#pragma once

#include "cell_id.h"
#include "fractional_counter.h"
#include "cell_size.h"

class object_cell_position
{

public:

  enum class move_direction { none, up, down, left, right };

  object_cell_position(cell_id position, float moveInterval);
  auto Set(cell_id position) -> void;
  auto operator()(float interval, move_direction moveDirection, cell_size cellSize) -> POINT_2F;

  [[nodiscard]] auto Current() const noexcept -> cell_id;
  [[nodiscard]] auto Next() const noexcept -> cell_id;

  [[nodiscard]] auto MoveComplete() const noexcept -> bool;

  static [[nodiscard]] auto MoveDirection(float direction) noexcept -> move_direction;
  static [[nodiscard]] auto MoveDirection(float x, float y) noexcept -> move_direction;

private:

  cell_id m_currentPosition;
  cell_id m_nextPosition;
  fractional_counter m_moveInterval;

};
