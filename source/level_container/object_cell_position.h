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

private:

  cell_id m_currentPosition;
  cell_id m_nextPosition;
  fractional_counter m_moveInterval;

};
