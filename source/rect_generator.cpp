#include "pch.h"
#include "rect_generator.h"

rect_generator::rect_generator(game_rect rect, int columnCount, int rowCount) : m_rect(rect), m_cellGenerator(columnCount, rowCount)
{
  m_cellWidth = m_rect.Width() / columnCount;
  m_cellHeight = m_rect.Height() / rowCount;
}

auto rect_generator::Get(std::back_insert_iterator<collection> trueInserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  cell_generator::collection cells;

  m_cellGenerator.Get(std::back_inserter(cells), noiseValueCheck);

  std::transform(cells.cbegin(), cells.cend(), trueInserter, [this](auto cellID) -> game_rect
  {
    return GetRect(cellID);
  });
}

auto rect_generator::Get(std::back_insert_iterator<collection> trueInserter, std::back_insert_iterator<collection> falseInserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  cell_generator::collection trueCells;
  cell_generator::collection falseCells;

  m_cellGenerator.Get(std::back_inserter(trueCells), std::back_inserter(falseCells), noiseValueCheck);

  std::transform(trueCells.cbegin(), trueCells.cend(), trueInserter, [this](auto cellID) -> game_rect
  {
    return GetRect(cellID);
  });

  std::transform(falseCells.cbegin(), falseCells.cend(), falseInserter, [this](auto cellID) -> game_rect
  {
    return GetRect(cellID);
  });
}

auto rect_generator::GetRect(cell_generator::cell_id cellID) const -> game_rect
{
  auto topLeft = game_point { m_cellWidth * cellID.column + m_rect.topLeft.x, m_cellHeight * cellID.row + m_rect.topLeft.y };
  auto bottomRight = game_point { topLeft.x + m_cellWidth, topLeft.y + m_cellHeight };
  return { topLeft, bottomRight };
}
