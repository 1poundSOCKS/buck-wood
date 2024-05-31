#pragma once

#include "level_base.h"
#include "game_world_cell_data_translator.h"
#include "game_world_object_data_translator.h"
#include "level_container.h"

class game_world
{

public:

  game_world();

  [[nodiscard]] auto LevelData(int index) const -> std::unique_ptr<level_base>;
  [[nodiscard]] auto EntryData(int index, POINT_2I exitCell) -> std::optional<std::tuple<int, POINT_2I>>;
  auto LoadLevel(int levelIndex, std::optional<POINT_2I> entryCell) const -> std::unique_ptr<level_container>;

private:

  static [[nodiscard]] auto CollisionType() -> collision_type;
  [[nodiscard]] auto CreateCellsCollection(int levelIndex, level_base* levelData) const -> std::shared_ptr<level_cell_collection>;
  auto CreateLevelLink(int exitLevelIndex, char exitCellDataValue, int entryLevelIndex, char entryCellDataValue) -> void;
  static auto SetCellId(portal& object, POINT_2I cellId) -> void;
  static auto SetCellId(auto&& object, POINT_2I cellId) -> void;
  static auto SetCells(player_ship& object, std::shared_ptr<level_cell_collection> cells) -> void;
  static auto SetCells(enemy_type_1& object, std::shared_ptr<level_cell_collection> cells) -> void;
  static auto SetCells(enemy_type_2& object, std::shared_ptr<level_cell_collection> cells) -> void;
  static auto SetCells(enemy_type_3& object, std::shared_ptr<level_cell_collection> cells) -> void;
  static auto SetCells(auto&& object, std::shared_ptr<level_cell_collection> cells) -> void;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  game_world_object_data_translator m_objectDataTranslator;
  std::map<std::tuple<int, int, int>,std::tuple<int, int, int>> m_links;
  collision_type m_collisionType;

};

inline auto game_world::SetCellId(portal &object, POINT_2I cellId) -> void
{
  object.SetCellId(cellId);
}

auto game_world::SetCellId(auto &&object, POINT_2I cellId) -> void
{
}

inline auto game_world::SetCells(player_ship &object, std::shared_ptr<level_cell_collection> cells) -> void
{
  object.SetCells(cells);
}

inline auto game_world::SetCells(enemy_type_1 &object, std::shared_ptr<level_cell_collection> cells) -> void
{
  object.SetCells(cells);
}

inline auto game_world::SetCells(enemy_type_2 &object, std::shared_ptr<level_cell_collection> cells) -> void
{
  object.SetCells(cells);
}

inline auto game_world::SetCells(enemy_type_3 &object, std::shared_ptr<level_cell_collection> cells) -> void
{
  object.SetCells(cells);
}

auto game_world::SetCells(auto&& object, std::shared_ptr<level_cell_collection> cells) -> void
{
}
