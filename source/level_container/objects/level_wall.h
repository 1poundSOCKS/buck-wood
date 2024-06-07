#pragma once

#include "base_object.h"
#include "level_types.h"

class level_wall : public base_object
{

public:

  level_wall(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) noexcept : 
    base_object { position, scale, angle }, m_type { level_cell_type::floor }
  {
  }

  auto SetType(level_cell_type cellType) -> void
  {
    m_type = cellType;
  }

  auto SetId(cell_id id) -> void
  {
    m_cellId = id;
  }

  [[nodiscard]] auto Type() const noexcept -> level_cell_type
  {
    return m_type;
  }

  [[nodiscard]] auto CellId() const noexcept -> cell_id
  {
    return m_cellId;
  }

private:

  level_cell_type m_type;
  cell_id m_cellId;

};
