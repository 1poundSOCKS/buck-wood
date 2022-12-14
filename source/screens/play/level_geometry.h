#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  std::vector<game_closed_object> objects;
  std::vector<game_point> allPoints;
  std::vector<game_line> allLines;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry;
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry;
[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool;
[[nodiscard]] auto GetLevelBoundary(const level_ground_geometry& groundGeometry) -> game_rect;

inline [[nodiscard]] auto HasCollided(float x, float y, const level_ground_geometry& groundGeometry) -> bool
{
  return IsUnderground(x, y, groundGeometry);
}

inline [[nodiscard]] auto HasCollided(float x, float y, const level_targets_geometry& geometry) -> bool
{
  return PointInside({ x, y}, geometry.lines.cbegin(), geometry.lines.cend());
}

#endif
