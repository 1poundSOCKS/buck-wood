#include "pch.h"
#include "boundary_data.h"

auto boundary_data::Add(int index, int cellWidth, int cellHeight) noexcept -> void
{
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyCellData(index, std::back_inserter(cellData));

  m_levelData.insert({index, cellData});

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, type] = cellDataItem;
    return type == level_data::cell_type::empty;
  });

  std::vector<POINT_2F> boundaryData;
  point_data::CellsToBoundary(emptyCells, cellWidth, cellHeight, std::back_inserter(boundaryData));

  m_boundaryData[index] = boundaryData;
}
