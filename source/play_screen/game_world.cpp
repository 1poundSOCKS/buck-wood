#include "pch.h"
#include "game_world.h"
#include "level_1.h"
#include "level_2.h"

game_world::game_world()
{
  auto levelData = LevelData(0);
  m_dataTranslator.SetLevelIndex(0);
  m_dataTranslator.EnumerateCells(levelData.get(), [this](size_t column, size_t row, level_cell_type cellType) -> void
  {
    switch( cellType )
    {
    case level_cell_type::exit:
      m_exits[0] = { static_cast<int>(column), static_cast<int>(row) };
      break;
    }
  });

  auto levelData1 = LevelData(1);
  m_dataTranslator.SetLevelIndex(1);
  m_dataTranslator.EnumerateItems(levelData1.get(), [this](size_t column, size_t row, level_item_type objectType) -> void
  {
    switch( objectType )
    {
    case level_item_type::portal:
      m_entries[1] = { static_cast<int>(column), static_cast<int>(row) };
      break;
    }
  });

  m_levelLinks[0] = 1;
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
