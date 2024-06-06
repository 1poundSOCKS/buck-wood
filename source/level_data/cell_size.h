#pragma once

#include "framework.h"
#include "cell_id.h"

class cell_size
{

public:

  cell_size(int width, int height);

  [[nodiscard]] auto CellPosition(cell_id cellId) const noexcept -> POINT_2I;
  [[nodiscard]] auto CellRect(cell_id cellId) const noexcept -> RECT_I;

private:

  int m_width;
  int m_height;

};
