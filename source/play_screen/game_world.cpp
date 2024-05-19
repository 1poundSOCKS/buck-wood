#include "pch.h"
#include "game_world.h"
#include "level_1.h"
#include "level_2.h"

game_world::game_world()
{
  std::map<int, POINT_2I> exits;
  std::map<int, POINT_2I> entries;

  auto levelData = LevelData(0);
  m_dataTranslator.SetLevelIndex(0);
  m_dataTranslator.EnumerateCells(levelData.get(), [&exits](size_t column, size_t row, level_cell_type cellType) -> void
  {
    switch( cellType )
    {
    case level_cell_type::exit:
      exits[0] = { static_cast<int>(column), static_cast<int>(row) };
      break;
    }
  });

  level_2 level2;
  level2.Enumerate([&entries](int column, int row, char data)
  {
    switch( data )
    {
    case 'E':
      entries[data] = { column, row };
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
  auto nextlevelIndex = LevelIndex(index, exitCell);
  auto levelData = LevelData(nextlevelIndex);

  POINT_2I entryCell { 0, 0 };

  m_dataTranslator.SetLevelIndex(index);
  m_dataTranslator.EnumerateItems(levelData.get(), [&entryCell](size_t column, size_t row, level_item_type itemType) -> void
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    
    switch( itemType )
    {
      case level_item_type::portal:
        entryCell = { columnIndex, rowIndex };
        break;

      default:
        break;
    }
  });

  return entryCell;
}
