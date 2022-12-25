#ifndef _level_geometry_
#define _level_geometry_

#include "game_level_data.h"

struct level_ground_geometry
{
  game_rect boundary;
  game_point groundStart;
  game_point groundEnd;
  std::vector<game_line> lines;
};

struct rect_underground_state
{
  enum underground_state_type { none, partial, all };
  game_rect rect;
  underground_state_type undergroundState;
};

struct level_grid_def
{
  int columnWidth;
  int rowHeight;
  int firstColumn;
  int columnCount;
  int firstRow;
  int rowCount;
};

struct level_rect_grid
{
  std::vector<std::vector<game_rect>> rects;
};

struct level_ground_matrix
{
  std::vector<std::vector<rect_underground_state>> undergroundFlags;
};

struct level_targets_geometry
{
  std::vector<game_point> points;
  std::vector<game_line> lines;
};

[[nodiscard]] auto CreateLevelGroundGeometry(const game_level_data& levelData) -> level_ground_geometry [[nothrow]];
[[nodiscard]] auto CreateLevelTargetsGeometry(const game_level_data& levelData) -> level_targets_geometry [[nothrow]];
[[nodiscard]] auto IsUnderground(float x, float y, const level_ground_geometry& levelGroundGeometry) -> bool [[nothrow]];
[[nodiscard]] auto IsUnderground(game_rect rect, const level_ground_geometry& groundGeometry) -> bool [[nothrow]];
[[nodiscard]] auto GetLevelGridDef(int columnWidth, int rowHeight, const level_ground_geometry& groundGeometry) -> level_grid_def [[nothrow]];
[[nodiscard]] auto CreateLevelRectGrid(level_grid_def gridDef) -> level_rect_grid [[nothrow]];
[[nodiscard]] auto CreateLevelGroundMatrix(const level_rect_grid& grid, const level_ground_geometry& groundGeometry) -> level_ground_matrix [[nothrow]];

#endif
