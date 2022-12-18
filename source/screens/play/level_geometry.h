#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_geometry
{
  std::vector<game_line> lines;
  game_point groundStart;
  game_point groundEnd;
};

[[nodiscard]] std::unique_ptr<level_geometry> CreateLevelGeometry(const game_level_data& levelData);
[[nodiscard]] bool CoordinateIsVisible(float x, float y, const level_geometry* levelGeometry);

#endif
