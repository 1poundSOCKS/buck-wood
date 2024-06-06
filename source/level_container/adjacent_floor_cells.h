#pragma once

#include "level_cell_collection.h"

class adjacent_floor_cells
{

public:

  adjacent_floor_cells(const level_cell_collection& cells, cell_id cellId);

  [[nodiscard]] auto Count() const noexcept -> size_t;
  [[nodiscard]] auto operator[](size_t index) -> cell_id;
  auto Enumerate(auto&& callable) -> void;

private:

  const level_cell_collection& m_cells;
  const cell_id m_cellId;
  std::vector<cell_id> m_adjacentCellIds;

};

auto adjacent_floor_cells::Enumerate(auto&& callable) -> void
{
  for( const auto& cell : m_adjacentCellIds )
  {
    callable(cell);
  }
}
