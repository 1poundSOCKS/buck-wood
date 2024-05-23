#pragma once

#include "base_object.h"
#include "level_types.h"

class level_wall : public base_object
{

public:

  level_wall(POINT_2F position, SCALE_2F scale, float angle, level_cell_type type) noexcept : 
    base_object { position, scale, angle }, m_type { type }
  {
  }

  [[nodiscard]] auto Type() const noexcept -> level_cell_type
  {
    return m_type;
  }

private:

  level_cell_type m_type;

};
