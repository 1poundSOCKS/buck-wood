#pragma once

#include "level_cell_collection.h"

class adjacent_cell_visitor
{

public:

  adjacent_cell_visitor(level_cell_collection::cell_id cellId) : m_cellId { cellId }
  {
  }

  auto operator()(const level_cell_collection& cells, auto&& callable) -> void
  {
    constexpr auto adjacentCellIds = std::array {
      level_cell_collection::cell_id(-1,-1),
      level_cell_collection::cell_id(0,-1),
      level_cell_collection::cell_id(1,-1),
      level_cell_collection::cell_id(-1,0),
      level_cell_collection::cell_id(0,0),
      level_cell_collection::cell_id(1,0),
      level_cell_collection::cell_id(-1,1),
      level_cell_collection::cell_id(0,1),
      level_cell_collection::cell_id(1,1)
    };

    const auto& [objectColumn, objectRow] = m_cellId;
    
    const auto& cellsMap = cells.Get();

    auto adjacentCellIterators = std::ranges::views::transform(adjacentCellIds, [&cellsMap,objectColumn,objectRow](const auto& cellId)
    {
      const auto& [column, row] = cellId;
      level_cell_collection::cell_id id = { column + objectColumn, row + objectRow };
      return cellsMap.find(id);
    });

    auto adjacentWallIterators = std::ranges::views::filter(adjacentCellIterators, [&cellsMap](const auto& cellIterator)
    {
      return cellIterator == std::end(cellsMap) ? false : true;
    });

  }

private:

  level_cell_collection::cell_id m_cellId;

};
