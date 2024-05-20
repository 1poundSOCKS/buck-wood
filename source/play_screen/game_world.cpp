#include "pch.h"
#include "game_world.h"
#include "level_1.h"
#include "level_2.h"

game_world::game_world()
{
  SaveLevelLink(0, 'E', 1, 'F');
}

auto game_world::LevelIndex(int index, POINT_2I exitCell) const -> int
{
  return m_levelLinks.at(index);
}

auto game_world::LevelData(int index) const -> std::unique_ptr<level_base>
{
  switch( index )
  {
    case 0:
      return std::make_unique<level_1>();
      
    case 1:
      return std::make_unique<level_2>();

    default:
      return std::make_unique<level_1>();
  }
}

auto game_world::EntryCell(int index, POINT_2I exitCell) -> POINT_2I
{
  auto nextLevelIndex = m_levelLinks[index];
  auto entryIterator = m_entries.find(nextLevelIndex);
  return entryIterator != std::end(m_entries) ? entryIterator->second : POINT_2I { 0, 0 };
}

auto game_world::SaveLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void
{
  auto levelData = LevelData(exitLevelIndex);

  levelData->Enumerate([this,exitLevelIndex,exitCellDataValue](size_t column, size_t row, char cellData)
  {
    if( cellData == exitCellDataValue )
    {
      m_exits[exitLevelIndex] = { static_cast<int>(column), static_cast<int>(row) };
    }
  });

  auto levelData1 = LevelData(entryLevelIndex);

  levelData1->Enumerate([this,entryLevelIndex,entryCellDataValue](size_t column, size_t row, char cellData)
  {
    if( cellData == entryCellDataValue )
    {
      m_entries[entryLevelIndex] = { static_cast<int>(column), static_cast<int>(row) };      
    }
  });

  m_levelLinks[exitLevelIndex] = entryLevelIndex;
}
