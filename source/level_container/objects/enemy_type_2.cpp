#include "pch.h"
#include "enemy_type_2.h"
#include "adjacent_floor_cells.h"

enemy_type_2::enemy_type_2(POINT_2F position, SCALE_2F scale, float angle, VELOCITY_2F velocity) : 
  enemy_object { position, scale, angle }, m_status { status::moving }, 
  m_waitTimer { 1 }, m_speed { 100 }, m_reloadTimer { 0.5f }, m_destination { std::nullopt }
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
  m_destination = atDestination ? std::nullopt : m_destination;
  return atDestination ? status::waiting : status::moving;
}

[[nodiscard]] auto enemy_type_2::UpdateWhenWaiting(float interval) noexcept -> status
{
  auto finishedWaiting = m_waitTimer.Update(interval);
  return finishedWaiting ? status::moving : status::waiting;
}

auto enemy_type_2::MoveTowardsDestination(level_cell_item destination, float interval) noexcept -> bool
{
  auto position = destination.Position();
  bool atDestination = MoveTowards(m_speed * interval, position);
  return atDestination;
}

auto enemy_type_2::NewDestination(const level_cell_collection& cells) -> std::optional<level_cell_item>
{
  auto cellId = cells.CellId(m_position);
  auto adjacentFloorCells = adjacent_floor_cells(cells, cellId);
  auto adjacentFloorCellsCount = adjacentFloorCells.Count();
  std::uniform_int_distribution<size_t> floorCellDistribution { 0, adjacentFloorCellsCount - 1 };
  auto randomCellIndex = floorCellDistribution(pseudo_random_generator::get());
  auto randomCellId = adjacentFloorCells[randomCellIndex];
  return cells.Get(randomCellId);
}
