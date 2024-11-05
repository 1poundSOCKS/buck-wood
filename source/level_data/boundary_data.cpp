#include "pch.h"
#include "boundary_data.h"

auto boundary_data::Add(int index, int cellWidth, int cellHeight) noexcept -> void
{
  m_boundaryData[index] = {};
  m_innerWallData[index] = {};
  
  std::vector<std::tuple<int, int, level_data::cell_type>> cellData;
  level_data::CopyCellData(index, std::back_inserter(cellData));

  auto bounds = Bounds(cellData);

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

  auto remainingLines = point_data::CellsIdsToBoundary(emptyCellIds, cellWidth, cellHeight, std::back_inserter(m_boundaryData[index]));

  auto&& innerWallData = m_innerWallData[index];

  while( remainingLines.size() )
  {
    innerWallData.resize(innerWallData.size() + 1);
    auto&& newInnerWall = innerWallData[innerWallData.size() - 1];
    std::vector<POINT_2F> innerBoundaryData;
    remainingLines = point_data::LinesToBoundary(remainingLines, cellWidth, cellHeight, std::back_inserter(newInnerWall));
  }
}
