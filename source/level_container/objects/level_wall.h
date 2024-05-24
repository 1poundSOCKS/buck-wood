#pragma once

#include "base_object.h"
#include "level_types.h"

class level_wall : public base_object
{

public:

  level_wall(POINT_2F position, SCALE_2F scale, float angle, level_cell_type type, POINT_2I cellId) noexcept : 
    base_object { position, scale, angle }, m_type { type }, m_cellId { cellId }
  {
  }

  [[nodiscard]] auto Type() const noexcept -> level_cell_type
  {
    return m_type;
  }

  [[nodiscard]] auto CellId() const noexcept -> POINT_2I
  {
    return m_cellId;
  }

private:

  level_cell_type m_type;
  POINT_2I m_cellId;

};
