#include "pch.h"
#include "game_world.h"
#include "level_1.h"
#include "level_2.h"
#include "level_data_translator.h"

game_world::game_world()
{
  // level_1 level1;
  // level_2 level2;

  // level1.Enumerate([]()
  // {

  // },
  // []()
  // {

  // });

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

  level_data_translator levelDataTranslator;
  levelDataTranslator.EnumerateItems(levelData.get(), [&entryCell](size_t column, size_t row, level_item_type itemType) -> void
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    
    switch( itemType )
    {
      case level_item_type::portal:
        entryCell = { columnIndex, rowIndex };
        break;
    }
  });

  return entryCell;
}
