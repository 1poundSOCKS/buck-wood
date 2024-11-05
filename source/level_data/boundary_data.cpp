#include "pch.h"
#include "boundary_data.h"

auto boundary_data::Add(int index, int cellWidth, int cellHeight) noexcept -> void
{
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyCellData(index, std::back_inserter(cellData));

  auto bounds = Bounds(cellData);

  m_levelData.insert({index, cellData});

  auto emptyCells = std::ranges::views::filter(cellData, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, type] = cellDataItem;
    return type == level_data::cell_type::empty;
  });

  auto emptyCellIds = std::ranges::views::transform(emptyCells, [](auto&& cellDataItem) -> POINT_2I
  {
    auto&& [column, row, value] = cellDataItem;
    return { column, row };
  });

  std::vector<POINT_2F> boundaryData;
  point_data::CellsIdsToOrderedBoundaryPoints(emptyCellIds, cellWidth, cellHeight, std::back_inserter(boundaryData));

  std::vector<std::vector<POINT_2F>> boundaries;
  point_data::CellsIdsToBoundaries(emptyCellIds, cellWidth, cellHeight, std::back_inserter(boundaries));

  m_boundaryData[index] = boundaryData;
}
