#pragma once

#include "level_grid.h"

class level_grid_cell_type
{
public:

  enum class type { empty, target, asteroid, mine };
  
  level_grid_cell_type(const level_grid_cell& cell);
  [[nodiscard]] auto Get() const -> type;
  [[nodiscard]] auto IsAsteroid() const -> bool;
  [[nodiscard]] auto IsTarget() const -> bool;
  [[nodiscard]] auto IsMine() const -> bool;

private:

  type m_type { type::empty };

};
