#pragma once

#include "cell_id.h"

class level_cell_item
{

public:

  level_cell_item(cell_id cellId, level_cell_type cellType, POINT_2I position) : 
    level_cell_item { cellId, cellType, POINT_2F { static_cast<float>(position.x), static_cast<float>(position.y) } }
  {
  }

  level_cell_item(cell_id cellId, level_cell_type cellType, POINT_2F position) :
    m_cellId { cellId }, m_cellType { cellType }, m_position { position }
  {
  }

  [[nodiscard]] auto CellId() const noexcept -> cell_id
  {
    return m_cellId;
  }

  [[nodiscard]] auto Type() const noexcept -> level_cell_type
  {
    return m_cellType;
  }

  [[nodiscard]] auto Position() const noexcept -> POINT_2F
  {
    return m_position;
  }

private:

  cell_id m_cellId;
  level_cell_type m_cellType;
  POINT_2F m_position;

};
