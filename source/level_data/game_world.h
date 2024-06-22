#pragma once

#include "level_base.h"
#include "game_world_cell_data_translator.h"
#include "game_world_object_data_translator.h"
#include "level_container.h"
#include "level_object_movement.h"

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryData(int index, cell_id exitCell) -> std::optional<std::tuple<int, cell_id>>;
  auto LoadLevel(int levelIndex, std::optional<cell_id> entryCell) const -> std::unique_ptr<level_container>;

private:

  static [[nodiscard]] auto CollisionType() -> collision_type;
  [[nodiscard]] auto CreateCellsCollection(int levelIndex, level_base* levelData, cell_size cellSize) const -> std::shared_ptr<level_cell_collection>;
  auto CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  game_world_object_data_translator m_objectDataTranslator;
  std::map<std::tuple<int, cell_id>,std::tuple<int, int, int>> m_links;
  collision_type m_collisionType;

};
