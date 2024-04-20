#include "pch.h"
#include "enemy_type_2.h"

enemy_type_2::enemy_type_2(POINT_2F position, int hitpoints, float speed, float reloadTime, valid_cell_collection cells) : 
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_speed { speed }, m_reloadTimer { reloadTime }
{
  auto validCells = std::ranges::views::filter(cells, [](const auto& cell)
  {
    auto [valid, x, y, position, geometry] = cell;
    return valid;
  });

  std::ranges::copy(validCells, std::back_inserter(m_cells));

  auto originCell = std::ranges::views::filter(cells, [](const auto& cell)
  {
    auto [valid, x, y, position, geometry] = cell;
    return x == 0 && y == 0;
  });

  m_destination = originCell.front();
}

auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);

  m_destination = m_destination ? m_destination : NewDestination();
  auto [a, x, y, position, b] = *m_destination;
  m_destination = MoveTowards(m_speed * interval, position) ? NewDestination() : m_destination;
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_2::NewDestination() -> valid_cell
{
  auto adjacentCells = std::ranges::views::filter(m_cells, [this](const auto& cell)
  {
    auto [destValid, destX, destY, destPosition, destGeometry] = *m_destination;
    auto [valid, x, y, position, geometry] = cell;
    auto minX = destX - 1;
    auto maxX = destX + 1;
    auto minY = destY - 1;
    auto maxY = destY + 1;
    return x >= minX && x <= maxX && y >= minY && y <= maxY;
  });

  size_t adjacentCellCount = std::ranges::distance(adjacentCells);

  if( adjacentCellCount == 0 )
  {
    return *m_destination;
  }
  else
  {
    std::uniform_int_distribution<size_t> cellDist { 0, adjacentCellCount - 1 };
    auto cellIndex = cellDist(pseudo_random_generator::get());
    auto cellIterator = std::begin(adjacentCells);
    std::advance(cellIterator, cellIndex);
    return *cellIterator;
  }
}
