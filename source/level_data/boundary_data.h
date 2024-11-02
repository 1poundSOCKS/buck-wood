#pragma once

#include "geometry/image_data.h"

class boundary_data
{

public:

  static auto create(int cellWidth, int cellHeight) noexcept -> void;
  static auto destroy() noexcept -> void;
  static auto get(int index) -> const std::vector<image_data::const_iterator::value_type>&;
  static auto getBoundary(int index) -> const std::vector<POINT_2F>&;
  static [[nodiscard]] auto indexIsValid(int index) noexcept -> bool;

  static [[nodiscard]] auto begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;
  static [[nodiscard]] auto end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator;

private:

  boundary_data(int cellWidth, int cellHeight);
  auto Add(int index, auto&& data, int cellWidth, int cellHeight) noexcept -> void;

  inline static boundary_data* m_instance { nullptr };
  std::map<int, std::vector<image_data::const_iterator::value_type>> m_levelData;
  std::map<int, std::vector<POINT_2F>> m_boundaryData;

};

inline auto boundary_data::create(int cellWidth, int cellHeight) noexcept -> void
{
  destroy();
  m_instance = new boundary_data(cellWidth, cellHeight);
}

inline auto boundary_data::destroy() noexcept -> void
{
  delete m_instance;
  m_instance = nullptr;
}

inline auto boundary_data::get(int index) -> const std::vector<image_data::const_iterator::value_type> &
{
  return m_instance->m_levelData[index];
}

inline auto boundary_data::getBoundary(int index) -> const std::vector<POINT_2F> &
{
  return m_instance->m_boundaryData[index];
}

inline auto boundary_data::indexIsValid(int index) noexcept -> bool
{
  return m_instance->m_levelData.contains(index);
}

inline auto boundary_data::begin() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::begin(m_instance->m_boundaryData);
}

inline auto boundary_data::end() noexcept -> std::map<int, std::vector<POINT_2F>>::const_iterator
{
  return std::end(m_instance->m_boundaryData);
}

inline boundary_data::boundary_data(int cellWidth, int cellHeight)
{
  Add(0, level_data::levelData_0, cellWidth, cellHeight);
  Add(1, level_data::levelData_1, cellWidth, cellHeight);
  Add(2, level_data::levelData_2, cellWidth, cellHeight);
}

auto boundary_data::Add(int index, auto &&data, int cellWidth, int cellHeight) noexcept -> void
{
  image_data imageData { data };
  std::pair<int, std::vector<image_data::const_iterator::value_type>> levelData;
  levelData.first = index;
  std::copy(std::begin(imageData), std::end(imageData), std::back_inserter(levelData.second));
  m_levelData.insert(levelData);

  auto emptyCells = std::ranges::views::filter(levelData.second, [](auto&& cellDataItem) -> bool
  {
    auto&& [column, row, value] = cellDataItem;
    return level_data::ConvertToCellData(value) == level_data::cell_type::empty;
  });

  using cell_id = std::pair<int,int>;

  auto emptyCellIds = std::ranges::views::transform(emptyCells, [](auto&& cellDataItem) -> cell_id
  {
    auto&& [column, row, value] = cellDataItem;
    return { column, row };
  });

  std::set<cell_id> cellIdLookup;
  std::ranges::copy(emptyCellIds, std::inserter(cellIdLookup, std::begin(cellIdLookup)));
  
  using line_data = std::pair<POINT_2I, POINT_2I>;
  std::vector<line_data> lineData;

  for( auto&& cellId : emptyCellIds )
  {
    auto&& [column, row] = cellId;

    auto leftId = cell_id { column - 1, row };
    auto aboveId = cell_id { column, row - 1 };
    auto rightId = cell_id { column + 1, row };
    auto belowId = cell_id { column, row + 1 };

    if( !cellIdLookup.contains(leftId) )
    {
      auto pixelLineStart = POINT_2I { column, row + 1 };
      auto pixelLineEnd = POINT_2I { column, row };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(aboveId) )
    {
      auto pixelLineStart = POINT_2I { column, row };
      auto pixelLineEnd = POINT_2I { column + 1, row };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(rightId) )
    {
      auto pixelLineStart = POINT_2I { column + 1, row };
      auto pixelLineEnd = POINT_2I { column + 1, row + 1 };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }

    if( !cellIdLookup.contains(belowId) )
    {
      auto pixelLineStart = POINT_2I { column + 1, row + 1 };
      auto pixelLineEnd = POINT_2I { column, row + 1 };
      lineData.emplace_back(pixelLineStart, pixelLineEnd);
    }
  }

  using point_data_group = std::tuple<std::optional<POINT_2I>, POINT_2I, std::optional<POINT_2I>>;
  using point_data_group_container = std::list<point_data_group>;

  std::map<POINT_2I, POINT_2I> pixelLines;
  std::list<POINT_2I> pointData;
  point_data_group_container pointDataGroups;
  point_data_group_container normalizedPointDataGroups;
  
  std::ranges::copy(lineData, std::inserter(pixelLines, std::begin(pixelLines)));

  auto currentLine = std::begin(pixelLines);
  auto cellSize = cell_size { cellWidth, cellHeight };

  while( currentLine != std::end(pixelLines) )
  {
    const auto& [pixelLineStart, pixelLineEnd] = *currentLine;
    auto pixelRect = cellSize.CellRect({pixelLineStart.x, pixelLineStart.y});
    pointData.emplace_back(pixelRect.left, pixelRect.top);
    pixelLines.erase(currentLine);
    currentLine = pixelLines.find(pixelLineEnd);
  }

  for( auto pointIter = std::begin(pointData); pointIter != std::end(pointData); ++pointIter )
  {
    auto previousPoint = ( pointIter == std::begin(pointData) ? std::nullopt : std::optional<POINT_2I>(*std::prev(pointIter)) );
    auto pointIterNext = std::next(pointIter);
    auto nextPoint = ( pointIterNext == std::end(pointData) ? std::nullopt : std::optional<POINT_2I>(*pointIterNext) );
    pointDataGroups.emplace_back(previousPoint, *pointIter, nextPoint);
  }

  std::ranges::copy_if(pointDataGroups, std::back_inserter(normalizedPointDataGroups), [](auto&& pointDataGroup)
  {
    auto&& [prev, current, next] = pointDataGroup;
    auto groupIsFull = prev != std::nullopt && next != std::nullopt;
    auto allXAreEqual = groupIsFull && prev->x == current.x && next->x == current.x;
    auto allYAreEqual = groupIsFull && prev->y == current.y && next->y == current.y;
    return allXAreEqual || allYAreEqual ? false : true;
  });

  std::vector<POINT_2F> floatingPointData;
  std::ranges::transform(normalizedPointDataGroups, std::back_inserter(floatingPointData), [](auto&& pointDataGroup) -> POINT_2F
  {
    auto&& [prev, current, next] = pointDataGroup;
    return ToFloat(current);
  });

  m_boundaryData[index] = floatingPointData;
}
