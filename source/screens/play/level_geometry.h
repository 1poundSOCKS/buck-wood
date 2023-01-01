#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  game_rect boundary;
  game_point groundStart;
  game_point groundEnd;
  std::vector<game_line> lines;
  std::vector<game_point> points;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]];
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry [[nothrow]];
[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool [[nothrow]];

#endif
