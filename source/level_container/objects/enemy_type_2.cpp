#include "pch.h"
#include "enemy_type_2.h"

enemy_type_2::enemy_type_2(POINT_2F position, int hitpoints, float speed, float reloadTime, valid_cell_collection cells) : 
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_speed { speed }, m_reloadTimer { reloadTime }
{
  auto validCells = std::ranges::views::filter(cells, [](const auto& cell)
  {
    auto [valid, position, geometry] = cell;
    return valid;
  });

  std::ranges::copy(validCells, std::back_inserter(m_cells));
}

auto enemy_type_2::Update(float interval) -> void
{
  base_object::Update(interval);

  m_destination = m_destination ? m_destination : NewDestination();
  m_destination = MoveTowards(m_speed * interval, *m_destination) ? NewDestination() : m_destination;
  m_reloaded = m_reloadTimer.Update(interval);
}

auto enemy_type_2::NewDestination() -> POINT_2F
{
  if( m_cells.size() == 0 )
  {
    return { 0, 0 };
  }
  else
  {
    std::uniform_int_distribution<size_t> cellDist { 0, m_cells.size() - 1 };
    auto cellIndex = cellDist(pseudo_random_generator::get());
    auto [a, position, b] = m_cells[cellIndex];
    return position;
  }
}
