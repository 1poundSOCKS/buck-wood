#pragma once

#include "framework.h"
#include "cell_collection.h"
#include "cell_size.h"
#include "level_cell_item.h"
#include "level_cell_bounds.h"

class level_cell_collection
{

public:

  struct column_def
  {
    int position;
    int start;
    int end;
  };

  struct row_def
  {
    int position;
    int start;
    int end;
  };

  level_cell_collection(cell_size cellSize, RECT_I bounds);

  [[nodiscard]] auto Get(cell_id cellId) const -> level_cell_item;

  auto Set(cell_id cellId, level_cell_type cellType) noexcept -> void;

  [[nodiscard]] auto CellSize() const -> cell_size;
  [[nodiscard]] auto CellType(POINT_2F position) const -> level_cell_type;
  [[nodiscard]] auto CellId(POINT_2F position) const -> cell_id;
  [[nodiscard]] auto CellRect(cell_id cellId) const -> RECT_F;

  [[nodiscard]] auto IsTypeOf(cell_id cellId, level_cell_type cellType) const noexcept -> bool;
  [[nodiscard]] auto CellPosition(cell_id cellId) const noexcept -> POINT_2F;

  [[nodiscard]] auto IsUnoccupied(cell_id cellId) const noexcept -> bool;
  auto SetAsOccupied(cell_id cellId) -> void;
  auto SetAsUnoccupied(cell_id cellId) -> void;

  auto Enumerate(auto&& visitor) const noexcept -> void;
  auto EnumerateColumns(auto&& visitor) const noexcept -> void;
  auto EnumerateRows(auto&& visitor) const noexcept -> void;

  [[nodiscard]] auto UnoccupiedFloorCellCount() const noexcept -> size_t;
  [[nodiscard]] auto UnnoccupiedFloorCell(size_t index) const noexcept -> cell_id;

  [[nodiscard]] auto CentrePoint() const noexcept -> POINT_2F;
  [[nodiscard]] auto Bounds() const noexcept -> level_cell_bounds;

private:

  auto SetWall(cell_id cellId, cell_id::relative_position position) noexcept -> void;

private:

  using occupied_cell_collection = std::set<cell_id>;

  cell_size m_cellSize;
  cell_collection m_cells;
  occupied_cell_collection m_occupiedCells;

};

auto level_cell_collection::Enumerate(auto &&visitor) const noexcept -> void
{
  auto cellCollection = std::vector<std::tuple<cell_id, level_cell_type>>();

  m_cells.Enumerate([this,&cellCollection](cell_id cellId, level_cell_type cellType)
  {
    cellCollection.emplace_back(cellId, cellType);
  });

  for( const auto& cellEntry : cellCollection )
  {
    const auto& [cellId, cellType] = cellEntry;
    visitor(cellId, cellType);
  }
}

auto level_cell_collection::EnumerateColumns(auto &&visitor) const noexcept -> void
{
  for( auto column = m_cells.FirstColumn(); column != m_cells.EndOfColumns(); ++column )
  {
    auto topCellId = m_cells.ColumnTop(column);
    auto bottomCellId = m_cells.ColumnBottom(column);

    auto topCellPosition = m_cellSize.CellPosition(topCellId);
    auto bottomCellPosition = m_cellSize.CellPosition(bottomCellId);

    column_def columnDef { topCellPosition.x, topCellPosition.y, bottomCellPosition.y };
    visitor(columnDef);
  }
}

auto level_cell_collection::EnumerateRows(auto &&visitor) const noexcept -> void
{
  for( auto row = m_cells.FirstRow(); row != m_cells.EndOfRows(); ++row )
  {
    auto leftCellId = m_cells.RowLeft(row);
    auto rightCellId = m_cells.RowRight(row);

    auto leftCellPosition = m_cellSize.CellPosition(leftCellId);
    auto rightCellPosition = m_cellSize.CellPosition(rightCellId);

    row_def columnDef { leftCellPosition.y, leftCellPosition.x, rightCellPosition.x };
    visitor(columnDef);
  }
}

inline auto level_cell_collection::UnoccupiedFloorCellCount() const noexcept -> size_t
{
  return m_cells.FloorCellCount() - m_occupiedCells.size();
}

inline auto level_cell_collection::UnnoccupiedFloorCell(size_t index) const noexcept -> cell_id
{
  cell_id unoccupiedFloorCellId;

  m_cells.Enumerate([&index,&unoccupiedFloorCellId](cell_id cellId, level_cell_type cellType)
  {
    unoccupiedFloorCellId = (cellType == level_cell_type::floor && index-- == 0) ? cellId : unoccupiedFloorCellId;
  });

  return unoccupiedFloorCellId;
}

inline auto level_cell_collection::CentrePoint() const noexcept -> POINT_2F
{
  auto bounds = Bounds();
  
  auto width = static_cast<float>(bounds.Right(m_cellSize) - bounds.Left(m_cellSize));
  auto height = static_cast<float>(bounds.Bottom(m_cellSize) - bounds.Top(m_cellSize));
  
  return { bounds.Left(m_cellSize) + width / 2, bounds.Top(m_cellSize) + height / 2 };
}

inline auto level_cell_collection::Bounds() const noexcept -> level_cell_bounds
{
  auto cellIds = std::ranges::views::transform(m_cells, [](const auto& cellEntry)
  {
    const auto& [cellId, cellType] = cellEntry;
    return cellId;
  });

  return level_cell_bounds { cellIds };
}
