#include "pch.h"
#include "rect_generator.h"
#include "perlin_simplex_noise.h"

rect_generator::rect_generator(game_rect rect, float cellDiameter) : m_rect(rect), m_cellDiameter(cellDiameter)
{
}

auto rect_generator::Get(std::back_insert_iterator<collection> trueInserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  cell_generator::collection cells;

  cell_generator cellGenerator(m_rect, m_cellDiameter, m_cellDiameter);
  cellGenerator.Get(std::back_inserter(cells));

  std::vector<game_rect> cellRects;
  std::transform(cells.cbegin(), cells.cend(), std::back_inserter(cellRects), [this](auto cellID) -> game_rect
  {
    return GetRect(cellID);
  });

  std::copy_if(cellRects.cbegin(), cellRects.cend(), trueInserter, [noiseValueCheck](auto cellRect) -> bool
  {
    auto rectCentre = cellRect.CentrePoint();
    auto noiseValue = psn::GetNoise(rectCentre.x, rectCentre.y);
    return noiseValueCheck(noiseValue);
  });
}

auto rect_generator::Get(std::back_insert_iterator<collection> trueInserter, std::back_insert_iterator<collection> falseInserter, std::function<bool(float)> noiseValueCheck) const -> void
{
  cell_generator::collection cells;

  cell_generator cellGenerator(m_rect, m_cellDiameter, m_cellDiameter);
  cellGenerator.Get(std::back_inserter(cells));

  std::vector<game_rect> cellRects;
  std::transform(cells.cbegin(), cells.cend(), std::back_inserter(cellRects), [this](auto cellID) -> game_rect
  {
    return GetRect(cellID);
  });

  std::copy_if(cellRects.cbegin(), cellRects.cend(), trueInserter, [noiseValueCheck](auto cellRect) -> bool
  {
    auto rectCentre = cellRect.CentrePoint();
    auto noiseValue = psn::GetNoise(rectCentre.x, rectCentre.y);
    return noiseValueCheck(noiseValue);
  });

  std::copy_if(cellRects.cbegin(), cellRects.cend(), falseInserter, [noiseValueCheck](auto cellRect) -> bool
  {
    auto rectCentre = cellRect.CentrePoint();
    auto noiseValue = psn::GetNoise(rectCentre.x, rectCentre.y);
    return !noiseValueCheck(noiseValue);
  });
}

auto rect_generator::GetRect(cell_generator::cell_id cellID) const -> game_rect
{
  auto topLeft = game_point { m_cellDiameter * cellID.column, m_cellDiameter * cellID.row };
  auto bottomRight = game_point { topLeft.x + m_cellDiameter, topLeft.y + m_cellDiameter };
  return { topLeft, bottomRight };
}
