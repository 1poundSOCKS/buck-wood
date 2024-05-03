#pragma once

#include "level_cell_collection.h"

class adjacent_floor_cells
{

public:

  adjacent_floor_cells(const level_cell_collection& cells, level_cell_collection::cell_id cellId);

  [[nodiscard]] auto Count() const noexcept -> size_t;
  [[nodiscard]] auto operator[](size_t index) -> const valid_cell&;

private:

  const level_cell_collection& m_cells;
  const level_cell_collection::cell_id m_cellId;

};
