#pragma once

#include "framework.h"
#include "cell_id.h"

class level_cell_size
{

public:

  level_cell_size(int width, int height);

  [[nodiscard]] auto CellPosition(cell_id cellId) const noexcept -> POINT_2I;

private:

  int m_width;
  int m_height;

};
