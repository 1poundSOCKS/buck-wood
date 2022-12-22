#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  std::vector<game_line> lines;
  game_point groundStart;
  game_point groundEnd;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry;
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry;

#endif
