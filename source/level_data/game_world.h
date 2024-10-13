#pragma once

#include "game_world_cell_data_translator.h"
#include "game_world_object_data_translator.h"
#include "level_container.h"
#include "level_object_movement.h"

class game_world
{

public:

  game_world();

  auto LoadLevel(int levelIndex) const -> std::unique_ptr<level_container>;
  auto UpdateLevel(level_container& levelContainer) const noexcept -> void;
  auto LoadLevelData(int index, auto inserter) const -> bool;

private:

  static [[nodiscard]] auto CollisionType() -> collision_type;

private:

  game_world_cell_data_translator m_cellDataTranslator;
  game_world_object_data_translator m_objectDataTranslator;
  std::map<std::tuple<int, cell_id>,std::tuple<int, int, int>> m_links;
  collision_type m_collisionType;

};

auto game_world::LoadLevelData(int index, auto inserter) const -> bool
{
  static std::array levelData { 
    std::string { "XX       XXXXXX      XX             XXX" },
    std::string { "X       XX   XX                       X" },
    std::string { "X  1   XXX    X                        " },
    std::string { "      XXXX                             " },
    std::string { "                                      X" },
    std::string { "                                     XX" },
    std::string { "                         3      XXXXXXX" },
    std::string { "                                   XXXX" },
    std::string { "                                      X" },
    std::string { "XX            P                       X" },
    std::string { "XXXXXXXXXX                             " },
    std::string { "XXXXXXXXXXXXX                          " },
    std::string { "XXXXXXX                                " },
    std::string { "XXX                                  XX" },
    std::string { "                              XXXXXXXXX" },
    std::string { "         2                         XXXX" },
    std::string { "                                     XX" },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                                       " },
    std::string { "                3             2        " },
    std::string { "        X                              " },
    std::string { "XX      X                              " },
    std::string { "XXXX    X                1            X" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "       XX    2    X                  XX" },
    std::string { "X     XXX         XX         XXXXXXXXXX" }
  };
    
  switch( index )
  {
    case 0:
      std::ranges::copy(levelData, inserter);
      return true;
      
    default:
      return false;
  }
}
