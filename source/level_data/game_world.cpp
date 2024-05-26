#include "pch.h"
#include "game_world.h"
#include "level_0.h"
#include "level_1.h"
#include "level_2.h"

game_world::game_world()
{
  CreateLevelLink(0, 'E', 1, 'P');
  CreateLevelLink(1, 'E', 0, 'P');
  CreateLevelLink(1, 'G', 2, 'P');
  CreateLevelLink(2, 'E', 1, 'P');
}

auto game_world::LevelData(int index) const -> std::unique_ptr<level_base>
{
  switch( index )
  {
    case 0:
      return std::make_unique<level_0>();
      
    case 1:
      return std::make_unique<level_1>();

    case 2:
      return std::make_unique<level_2>();

    default:
      return std::make_unique<level_0>();
  }
}

auto game_world::EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>
{
  auto entryIterator = m_links.find({index,exitCell.x, exitCell.y});
  if( entryIterator == std::end(m_links) )
  {
    return std::nullopt;
  }
  else
  {
    const auto& [key, value] = *entryIterator;
    const auto& [levelIndex, column, row] = value;
    return std::make_optional<std::tuple<int, POINT_2I>>(levelIndex, POINT_2I { column, row });
  }
}

auto game_world::CreateCellsCollection(int levelIndex, level_base *levelData) const -> std::shared_ptr<level_cell_collection>
{
  std::shared_ptr<level_cell_collection> levelCells { std::make_shared<level_cell_collection>(400, 400) };

  levelData->Enumerate([this,levelIndex,levelCells](size_t column, size_t row, char cellData)
  {
    auto columnIndex = static_cast<int>(column);
    auto rowIndex = static_cast<int>(row);
    auto cellType = m_cellDataTranslator(levelIndex, cellData);

    switch( cellType )
    {
      case level_cell_type::floor:
        levelCells->Add(columnIndex, rowIndex, cellType);
        break;
      case level_cell_type::exit:
        levelCells->Add(columnIndex, rowIndex, cellType);
        break;
    }
  });

  levelCells->AddWalls();

  return levelCells;
}

auto game_world::CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void
{
  m_cellDataTranslator.AddExit(exitLevelIndex, exitCellDataValue);

  auto levelData = LevelData(exitLevelIndex);
  std::optional<POINT_2I> exitCell;

  levelData->Enumerate([this,exitLevelIndex,exitCellDataValue,&exitCell](size_t column, size_t row, char cellData)
  {
    if( cellData == exitCellDataValue )
    {
      exitCell = POINT_2I { static_cast<int>(column), static_cast<int>(row) };
    }
  });

  auto levelData1 = LevelData(entryLevelIndex);
  std::optional<POINT_2I> entryCell;

  levelData1->Enumerate([this,entryLevelIndex,entryCellDataValue,&entryCell](size_t column, size_t row, char cellData)
  {
    if( cellData == entryCellDataValue )
    {
      entryCell = POINT_2I { static_cast<int>(column), static_cast<int>(row) };
    }
  });

  if( exitCell && entryCell )
  {
    m_links[std::make_tuple(exitLevelIndex, exitCell->x, exitCell->y)] = std::make_tuple(entryLevelIndex, entryCell->x, entryCell->y);
  }
}
