#include "pch.h"
#include "enemy_type_2.h"
#include "adjacent_floor_cells.h"

enemy_type_2::enemy_type_2(POINT_2F position, int hitpoints, float waitTime, float speed, float reloadTime) : 
  enemy_object { position, { 1.5, 1.5 }, 0, hitpoints }, m_status { status::moving }, m_waitTimer { waitTime }, m_speed { speed }, m_reloadTimer { reloadTime }, m_destination { std::nullopt }
{
}

auto enemy_type_2::Update(float interval, const level_cell_collection& cells) -> void
{
  base_object::Update(interval);
  m_reloaded = m_reloadTimer.Update(interval);

  switch( m_status )
  {
    case status::moving:
      m_status = UpdateWhenMoving(interval, cells);
      break;
    case status::waiting:
      m_status = UpdateWhenWaiting(interval);
      break;
  }
}

[[nodiscard]] auto enemy_type_2::UpdateWhenMoving(float interval, const level_cell_collection& cells) noexcept -> status
{
  m_waitTimer.Reset();
  m_destination = m_destination ? m_destination : NewDestination(cells);
  bool atDestination = m_destination ? MoveTowardsDestination(*m_destination, interval) : true;
  return atDestination ? status::waiting : status::moving;
}

[[nodiscard]] auto enemy_type_2::UpdateWhenWaiting(float interval) noexcept -> status
{
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}

auto enemy_type_2::MoveTowardsDestination(valid_cell destination, float interval) noexcept -> bool
{
  auto position = destination.Position();
  bool atDestination = MoveTowards(m_speed * interval, position);
  m_destination = atDestination ? std::nullopt : m_destination;
  return atDestination;
}

auto enemy_type_2::NewDestination(const level_cell_collection& cells) -> std::optional<valid_cell>
{
  auto cellId = cells.CellId(m_position);
  auto adjacentFloorCells = adjacent_floor_cells(cells, cellId);
  auto adjacentFloorCellsCount = adjacentFloorCells.Count();
  std::uniform_int_distribution<size_t> floorCellDistribution { 0, adjacentFloorCellsCount - 1 };
  auto randomCellIndex = floorCellDistribution(pseudo_random_generator::get());
  auto randomCellId = adjacentFloorCells[randomCellIndex];
  return cells.Get(randomCellId);
}
